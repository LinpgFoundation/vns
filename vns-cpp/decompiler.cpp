#include <fstream>
#include <vector>
#include "naming.hpp"
#include "decompiler.hpp"


// If input string is nullptr, convert it to "null"
std::string Decompiler::to_str_in_case_null(const std::string &text)
{
    return text.empty() ? "null" : text;
}

void Decompiler::decompile(const std::unordered_map<std::string, std::any> &data, const std::filesystem::path &out_path)
{
    // If data is empty, return directly
    if (data.empty())
        return;

    // Initialize visual novel data management module
    DialoguesManager content_manager;
    // Update data to the management module
    content_manager.set_data(std::any_cast<DialogueSectionsDataType>(data.at("dialogs")));

    // string stream to store results
    std::stringstream result_ss;

    // add comment for Fundamental parameters
    result_ss << "# Fundamental parameters\n";
    // create id line
    result_ss << TAG_STARTS << "id" << TAG_ENDS << to_str_in_case_null(std::any_cast<std::string>(data.at("id")))
              << "\n";
    // create language line
    result_ss << TAG_STARTS << "language" << TAG_ENDS
              << to_str_in_case_null(std::any_cast<std::string>(data.at("language"))) << "\n";

    // Iterate through all sections
    for (const std::string &k: data | std::views::keys)
    {
        // Update the current position of the visual novel data management module
        content_manager.set_id("head");
        content_manager.set_section(k);

        // create section line
        result_ss << "\n" << TAG_STARTS << "section" << TAG_ENDS << k << "\n";

        while (true)
        {
            const DialogueDataType &current_dialog = content_manager.get_current_dialogue();
            // Process comments
            if (const auto &comments = std::get<std::vector<std::string>>(
                        current_dialog.at("comments")); !comments.empty())
            {
                result_ss << "\n";
                for (const std::string &note: comments)
                    result_ss << "// " << note << "\n";
            }
            // Write speaker's name
            result_ss << (content_manager.get_current()->narrator.empty() ? "null"
                                                                          : content_manager.get_current()->narrator)
                      << ":\n";
            // Write dialogue
            for (const auto &sentence: std::get<std::vector<std::string>>(current_dialog.at("contents")))
                result_ss << "- " << sentence << "\n";

            // If the following content has changed, write it
            // Write background
            if (content_manager.get_previous() == nullptr ||
                content_manager.get_current()->background_image != content_manager.get_previous()->background_image)
            {
                if (content_manager.get_previous() == nullptr ||
                    content_manager.get_previous()->next.get_type() != "scene")
                    result_ss << TAG_STARTS << "bgi" << TAG_ENDS
                              << to_str_in_case_null(content_manager.get_current()->background_image) << "\n";
                else
                    result_ss << TAG_STARTS << "scene" << TAG_ENDS
                              << to_str_in_case_null(content_manager.get_current()->background_image) << "\n";
            }
            // Write background music
            if (content_manager.get_previous() == nullptr ||
                content_manager.get_current()->background_music != content_manager.get_previous()->background_music)
            {
                result_ss << TAG_STARTS << "bgm" << TAG_ENDS
                          << to_str_in_case_null(content_manager.get_current()->background_music) << "\n";
            }
            // Write current character images
            if (content_manager.get_previous() == nullptr ||
                content_manager.get_current()->character_images != content_manager.get_previous()->character_images)
            {
                if (content_manager.get_current()->character_images.empty())
                    result_ss << TAG_STARTS << "hide" << TAG_ENDS << "*\n";
                else if (content_manager.get_previous() == nullptr ||
                         content_manager.get_previous()->character_images.empty())
                {
                    result_ss << TAG_STARTS << "display" << TAG_ENDS;
                    for (const std::string &character_name: content_manager.get_current()->character_images)
                        result_ss << Naming(character_name).get_name() << " ";
                    result_ss << "\n";
                }
            }

            if (content_manager.get_current()->has_next())
            {
                if (content_manager.get_current()->next.has_single_target())
                {
                    content_manager.set_id(content_manager.get_current()->next.get_target());
                } else
                {
                    // branch is currently not supported
                }
            } else
            {
                break;
            }
        }
    }

    // Write the stop symbol
    result_ss << "\n" << TAG_STARTS << "end" << TAG_ENDS << "\n";

    // Save the decompiled script
    std::ofstream out(out_path);
    out << result_ss.str();
    out.close();
}

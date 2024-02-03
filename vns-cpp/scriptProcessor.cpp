#include <vector>
#include <cassert>
#include <stdexcept>
#include <fstream>
#include "functions.h"
#include "scriptProcessor.h"
#include "naming.h"

std::string ScriptProcessor::get_id() const
{
    return id_;
}

std::string ScriptProcessor::get_language() const
{
    return lang_;
}

std::string ScriptProcessor::ensure_not_null(const std::string &text)
{
    return iequals(text, "null") || iequals(text, "none") ? "" : text;
}

std::string ScriptProcessor::extract_parameter(const std::string &text)
{
    return ensure_not_null(extract_string(text));
}

std::string ScriptProcessor::extract_tag(const std::string &text)
{
    return text.substr(text.find(TAG_STARTS) + 1, text.find(TAG_ENDS) - 1);
}

std::string ScriptProcessor::extract_string(const std::string &text)
{
    return trim(text.substr(text.find(TAG_ENDS) + 1));
}

[[noreturn]] void ScriptProcessor::terminated(const std::string &reason) const
{
    throw std::runtime_error(
            "File \"" + path_.string() + "\", line " + std::to_string(line_index_ + 1) + "\n  " + get_current_line() +
            "\nFail to compile due to: " + reason);
}

[[noreturn]] void ScriptProcessor::preprocess_terminated(const std::string &reason) const
{
    throw std::runtime_error("File \"" + path_.string() + "\" failed to compile due to: " + reason);
}

std::string ScriptProcessor::get_current_line() const
{
    return lines_[line_index_];
}

DialogueDataType ScriptProcessor::get_output() const
{
    DialogueDataType output;
    for (const auto &[section_name, section_contents]: output_)
    {
        output[section_name] = {};
        for (const auto &[dialogue_id, dialogue_content]: section_contents)
        {
            output[section_name][dialogue_id] = dialogue_content.to_map();
        }
    }
    return output;
}

nlohmann::json ScriptProcessor::get_output_as_json() const
{
    nlohmann::json output;
    for (const auto &[section_name, section_contents]: output_)
    {
        nlohmann::json section;
        for (const auto &[dialogue_id, dialogue_content]: section_contents)
        {
            section[dialogue_id] = dialogue_content.to_json();
        }
        output[section_name] = section;
    }
    return output;
}


void ScriptProcessor::process(const std::filesystem::path &path)
{
    path_ = path;
    int current_index = 0;

    //make sure file exists
    if (!exists(path_))
    {
        preprocess_terminated("File does not exist!");
    }

    // read file
    if (path_.extension().compare(SCRIPTS_FILE_EXTENSION) == 0)
    {
        if (std::ifstream f(path_); f.is_open())
        {
            std::string line;
            while (std::getline(f, line))
            {
                lines_.push_back(line);
            }
            f.close();
        } else
        {
            preprocess_terminated("File is occupied!");
        }
    }

    // make sure file is not empty
    if (lines_.empty())
    {
        preprocess_terminated("Cannot convert an empty script file!");
    }

    std::string last_label;

    for (size_t index = 0; index < lines_.size(); ++index)
    {
        lines_[index] = trim(lines_[index].substr(0, lines_[index].find(NOTE_PREFIX)));

        if (lines_[index].find(TAG_STARTS) == 0)
        {
            if (auto tag = extract_tag(lines_[index]); tag == "label")
            {
                if (!last_label.empty())
                {
                    terminated("This label is overwriting the previous one");
                }

                last_label = extract_parameter(lines_[index]);
                if (RESERVED_WORDS.contains(last_label))
                {
                    terminated("You cannot use reserved word '" + last_label + "' as a label");
                }
            } else if (tag == "section")
            {
                current_index = 0;
            }
        } else if (lines_[index].find(':') != std::string::npos)
        {
            dialog_associate_key_[index] =
                    current_index == 0 ? "head" : last_label.empty() ? (current_index < 10 ? "~0" + std::to_string(
                            current_index) : "~" + std::to_string(current_index)) : last_label;
            last_label = "";
            ++current_index;
        }
    }

    convert(0);
    lines_.clear();

    // making sure essential instances are init correctly
    if (id_.empty())
    {
        terminated("You have to set a id!");
    }
    if (lang_.empty())
    {
        terminated("You have to set lang!");
    }
    if (section_.empty())
    {
        terminated("You have to set section!");
    }
}

void ScriptProcessor::convert(const int starting_index)
{
    line_index_ = starting_index;

    while (line_index_ < lines_.size())
    {
        if (std::string current_line = get_current_line(); current_line.empty() ||
                                                           lines_[line_index_].find(NOTE_PREFIX) == 0)
        {
            // Skip empty lines or lines starting with #
            // Do nothing
        } else if (current_line.find(COMMENT_PREFIX) == 0)
        {
            // Accumulate comments
            accumulated_comments_.push_back(current_line.substr(COMMENT_PREFIX.length() + 1));
        } else if (current_line.find(TAG_STARTS) == 0)
        {
            std::string tag = extract_tag(current_line);

            if (ALTERNATIVES.contains(tag))
            {
                tag = ALTERNATIVES.at(tag);
            }

            if (tag == "bgi")
            {
                current_data_.background_image = extract_parameter(current_line);
            } else if (tag == "bgm")
            {
                current_data_.background_music = extract_parameter(current_line);
            } else if (tag == "show")
            {
                for (const std::string &name: split(extract_string(current_line)))
                {
                    current_data_.character_images.push_back(name);
                }
            } else if (tag == "hide")
            {
                for (const std::string &name: split(extract_string(current_line)))
                {
                    if (name == "*")
                    {
                        current_data_.character_images.clear();
                        break;
                    }
                    std::erase_if(current_data_.character_images, [&](const std::string &n) {
                        return Naming(n).equal(name);
                    });
                }
            } else if (tag == "display")
            {
                current_data_.character_images.clear();
                for (const std::string &name: split(extract_string(current_line)))
                {
                    current_data_.character_images.push_back(name);
                }
            } else if (tag == "id")
            {
                id_ = extract_parameter(current_line);
                if (id_.empty())
                {
                    terminated("Chapter id cannot be None!");
                }
            } else if (tag == "language")
            {
                lang_ = extract_string(current_line);
            } else if (tag == "section")
            {
                if (!previous_.empty())
                {
                    output_[section_][previous_].next = kNullContentNext;
                }
                section_ = extract_string(current_line);
                output_[section_] = {};
                output_[section_]["head"] = Content({}, "head");
                current_data_ = Content({}, "head");
                previous_ = "";
            } else if (tag == "end")
            {
                assert(!previous_.empty());
                output_[section_][previous_].next = kNullContentNext;
            } else if (tag == "scene")
            {
                assert(!previous_.empty());
                output_[section_][previous_].next = output_[section_][previous_].next.has_multi_targets() ? ContentNext(
                        "scene", output_[section_][previous_].next.get_targets()) : ContentNext("scene",
                                                                                                output_[section_][previous_].next.get_target());
                current_data_.background_image = extract_parameter(current_line);
                blocked_ = true;
            } else if (tag == "block")
            {
                if (!previous_.empty())
                {
                    output_[section_][previous_].next = kNullContentNext;
                }
                current_data_ = Content({}, "id_needed");
                previous_ = "";
            } else if (tag == "option")
            {
                if (current_data_.contents.empty())
                {
                    terminated("Invalid option syntax: '->' cannot be found!");
                }
                // get current targets
                MultiTargetsType current_targets;
                if (output_[section_][previous_].next.get_type() == "options")
                {
                    current_targets = output_[section_][previous_].next.get_targets();
                }
                // get value string
                auto src_to_target = extract_string(current_line);
                // push text and id map
                current_targets.push_back({{"text", trim(src_to_target.substr(0, src_to_target.find("->")))},
                                           {"id",   ensure_not_null(
                                                   trim(src_to_target.substr(src_to_target.find("->") + 2)))}});
                // update next
                output_[section_][previous_].next = ContentNext("options", current_targets);
            }
                // Placeholder, no action needed for "label" tag
            else if (tag == "label")
            {
            } else
            {
                terminated("Invalid tag " + tag);
            }
        } else if (current_line.find(':') != std::string::npos)
        {
            const std::string narrator = ensure_not_null(current_line.substr(0, current_line.size() - 1));
            current_data_.narrator = narrator;

            // Get the name of the narrator's possible image
            std::unordered_set<std::string> narrator_possible_images;
            if (std::string narrator_lower_case = to_lower(current_data_.narrator); Naming::get_database().contains(
                    narrator_lower_case))
            {
                narrator_possible_images = Naming::get_database()[narrator_lower_case];
            }
            for (auto &character_image: current_data_.character_images)
            {
                Naming name_data(character_image);
                if (narrator_possible_images.contains(name_data.get_name()))
                {
                    name_data.erase_tag("silent");
                } else
                {
                    name_data.insert_tag("silent");
                }
                character_image = name_data.to_string();
            }

            current_data_.contents.clear();

            for (size_t sub_index = line_index_ + 1; sub_index < lines_.size(); ++sub_index)
            {
                if (lines_[sub_index].find("- ") == 0)
                {
                    current_data_.contents.push_back(trim(lines_[sub_index].substr(2)));
                } else
                {
                    break;
                }
            }

            if (section_.empty())
            {
                terminated("You have to specify section before script");
            }
            if (!output_.contains(section_))
            {
                output_[section_] = {};
            }

            if (!previous_.empty())
            {
                if (!blocked_)
                {
                    current_data_.previous = previous_;
                } else
                {
                    current_data_.previous = "";
                    blocked_ = false;
                }

                if (output_[section_].contains(previous_))
                {
                    if (output_[section_][previous_].has_next())
                    {
                        if (output_[section_][previous_].next.get_type() != "options")
                        {
                            output_[section_][previous_].next = ContentNext(
                                    output_[section_][previous_].next.get_type(), dialog_associate_key_[line_index_]);
                        }
                    } else
                    {
                        output_[section_][previous_].next = ContentNext(output_[section_][previous_].next.get_type(),
                                                                        dialog_associate_key_[line_index_]);
                    }
                } else
                {
                    terminated("KeyError: " + previous_);
                }
            } else
            {
                current_data_.previous = "";
            }

            if (!accumulated_comments_.empty())
            {
                current_data_.comments = accumulated_comments_;
                accumulated_comments_.clear();
            }

            previous_ = dialog_associate_key_[line_index_];
            line_index_ += current_data_.contents.size();
            output_[section_][previous_] = Content(current_data_.to_map(), previous_);
            current_data_.comments.clear();
        } else
        {
            terminated("Invalid code or content!");
        }
        // Move to the next line
        ++line_index_;
    }
}

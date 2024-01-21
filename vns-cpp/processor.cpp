#include <string>
#include <vector>
#include "processor.h"
#include <stdexcept>
#include "functions.h"


int Processor::get_id() const
{
	return id_;
}

std::string Processor::get_language() const
{
	return lang_;
}

std::string Processor::ensure_not_null(const std::string& text)
{
	return (iequals(text, "null") || iequals(text, "none")) ? "" : text;
}

std::string Processor::extract_parameter(const std::string& text)
{
	return ensure_not_null(extract_string(text));
}

std::string Processor::extract_tag(const std::string& text)
{
	return text.substr(text.find(TAG_STARTS) + 1, text.find(TAG_ENDS));
}

std::string Processor::extract_string(const std::string& text)
{
	return trim(text.substr(text.find(TAG_ENDS) + 1));
}

void Processor::terminated(const std::string& reason) const
{
	throw std::runtime_error("File \"" + path_in_ + "\", line " + std::to_string(line_index_ + 1) +
		"\n  " + get_current_line() + "\nFail to compile due to " + reason);
}

std::string Processor::get_current_line() const
{
	return lines_[line_index_];
}

std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::any>>>
Processor::get_output() const
{
	return output_;
}

/*
void Processor::process(const std::string& path) {
    path_in_ = path;
    int current_index = 0;

    if (path_in_.ends_with(SCRIPTS_FILE_EXTENSION)) {
        std::ifstream file(path_in_);
        std::string line;
        while (std::getline(file, line)) {
            lines_.emplace_back(line);
        }
    }

    if (lines_.empty()) {
        terminated("Cannot convert an empty script file!");
    }

    std::string last_label;

    for (int index = 0; index < lines_.size(); ++index) {
        lines_[index] = trim(lines_[index].substr(0, lines_[index].find("#")));

        if (lines_[index].starts_with(TAG_STARTS)) {
            if (auto tag = extract_tag(lines_[index]); tag == "label") {
                if (!last_label.empty()) {
                    terminated("This label is overwriting the previous one");
                }

                last_label = extract_parameter(lines_[index]);
                if (RESERVED_WORDS.contains(last_label)) {
                    terminated("You cannot use reserved word '" + last_label + "' as a label");
                }
            }
            else if (tag == "section") {
                current_index = 0;
            }
        }
        else if (lines_[index].find(":") != std::string::npos) {
            dialog_associate_key_[index] = (current_index == 0 ? "head" : (current_index < 10 ? "~0" + std::to_string(current_index) : "~" + std::to_string(current_index)));

            last_label = "";
            ++current_index;
        }
    }

    convert(0);
    lines_.clear();

    if (id_ < 0) {
        terminated("You have to set a nonnegative id!");
    }
    else if (lang_.empty()) {
        terminated("You have to set lang!");
    }
    else if (section_.empty()) {
        terminated("You have to set section!");
    }
}

void Processor::convert(int starting_index) {
    line_index_ = starting_index;

    while (line_index_ < lines_.size()) {
        std::string currentLine = get_current_line();

        if (currentLine.empty() || lines_[line_index_].find("#") == 0) {
            // Skip empty lines or lines starting with #
            // Do nothing
        }
        else if (currentLine.find("//") == 0) {
            // Accumulate comments
            accumulated_comments_.push_back(currentLine.substr(2));
        }
        else if (currentLine.find(TAG_STARTS) == 0) {
            std::string tag = extract_tag(currentLine);

            switch (ALTERNATIVES.at(tag)) {
            case "bgi": {
                current_data_.background_image_ = extract_parameter(currentLine);
                break;
            }
            case "bgm": {
                current_data_.background_music_ = extract_parameter(currentLine);
                break;
            }
            case "show": {
                auto names = extract_string(currentLine);
                std::istringstream iss(names);
                std::copy(std::istream_iterator<std::string>(iss),
                    std::istream_iterator<std::string>(),
                    std::back_inserter(current_data_.character_images_));
                break;
            }
            case "hide": {
                auto names = extract_string(currentLine);
                if (names == "*") {
                    current_data_.character_images_.clear();
                }
                else {
                    current_data_.character_images_.erase(
                        std::remove_if(current_data_.character_images_.begin(),
                            current_data_.character_images_.end(),
                            [&](const std::string& name) {
                                return Naming(name).Equal(names);
                            }),
                        current_data_.character_images_.end());
                }
                break;
            }
            case "display": {
                current_data_.character_images_.clear();
                auto names = extract_string(currentLine);
                std::istringstream iss(names);
                std::copy(std::istream_iterator<std::string>(iss),
                    std::istream_iterator<std::string>(),
                    std::back_inserter(current_data_.character_images_));
                break;
            }
            case "id": {
                std::string id_str = extract_parameter(currentLine);
                if (!id_str.empty()) {
                    id_ = std::stoi(id_str);
                }
                else {
                    terminated("Chapter id cannot be None!");
                }
                break;
            }
            case "language": {
                lang_ = extract_string(currentLine);
                break;
            }
            case "section": {
                if (!previous_.empty()) {
                    output_[section_][previous_]["next"] = nullptr;
                }
                section_ = extract_string(currentLine);
                output_[section_] = {};
                output_[section_]["head"] = {};
                current_data_ = Content({}, "head");
                previous_ = "";
                break;
            }
            case "end": {
                assert(!previous_.empty());
                output_[section_][previous_]["next"] = nullptr;
                break;
            }
            case "scene": {
                assert(!previous_.empty());
                output_[section_][previous_]["next"]["type"] = "scene";
                current_data_.background_image_ = extract_parameter(currentLine);

                if (!current_data_.background_image_.empty() && current_data_.background_image_.length() == 0) {
                    current_data_.background_image_ = nullptr;
                }
                blocked_ = true;
                break;
            }
            case "block": {
                if (!previous_.empty()) {
                    output_[section_][previous_]["next"] = nullptr;
                }
                current_data_ = Content({}, "id_needed");
                previous_ = "";
                break;
            }
            case "option": {
                if (current_data_.contents_.empty()) {
                    terminated("Invalid option syntax: '->' cannot be found!");
                }

                assert(!previous_.empty());
                if (output_[section_][previous_]["next"] == nullptr) {
                    output_[section_][previous_]["next"] = {};
                }

                auto& next_ref = output_[section_][previous_]["next"];
                if (next_ref["type"] != "options") {
                    next_ref["type"] = "options";
                    next_ref["target"] = std::vector<std::unordered_map<std::string, std::variant<std::string, int>>>();
                }

                auto src_to_target = extract_string(currentLine);
                auto& target_list = std::get<std::vector<std::unordered_map<std::string, std::variant<std::string, int>>>>(next_ref["target"]);

                target_list.push_back({
                    {"text", src_to_target.substr(0, src_to_target.find("->")).strip()},
                    {"id", ensure_not_null(src_to_target.substr(src_to_target.find("->") + 2).strip())}
                    });

                break;
            }
            case "label": {
                // Placeholder, no action needed for "label" tag in C++ implementation
                break;
            }
            default: {
                terminated("Invalid tag " + tag);
                break;
            }
            }
        }
        else if (currentLine.find(":") != std::string::npos) {
            std::string narrator = ensure_not_null(currentLine.substr(0, currentLine.size() - 1));
            current_data_.narrator_ = narrator;

            // Rest of the logic for processing dialog content
        narrator_possible_images: tuple[str, ...] = tuple()
            if (current_data_.narrator_.lower() in Naming.get_database()) :
                narrator_possible_images = tuple(
                    Naming.get_database()[current_data_.narrator_.lower()]
                )
                for (int i = 0; i < current_data_.character_images_.size(); ++i) {
                    Naming name_data(current_data_.character_images_[i]);
                    if (std::find(narrator_possible_images.begin(), narrator_possible_images.end(), name_data.GetName()) != narrator_possible_images.end()) {
                        if (name_data.GetTags().count("silent")) {
                            name_data.GetTags().erase("silent");
                        }
                    }
                    else {
                        name_data.GetTags().insert("silent");
                    }
                    current_data_.character_images_[i] = name_data.ToString();
                }

        current_data_.contents_.clear();

        for (int sub_index = line_index_ + 1; sub_index < lines_.size(); ++sub_index) {
            if (lines_[sub_index].find("- ") == 0) {
                current_data_.contents_.push_back(lines_[sub_index].substr(2).strip());
            }
            else {
                break;
            }
        }

        if (section_.empty()) {
            terminated("You have to specify section before script");
        }
        else if (output_.find(section_) == output_.end()) {
            output_[section_] = {};
        }

        if (!previous_.empty()) {
            if (!blocked_) {
                current_data_.previous_ = previous_;
            }
            else {
                current_data_.previous_ = nullptr;
                blocked_ = false;
            }

            auto last_ref = output_[section_].find(previous_);
            if (last_ref != output_[section_].end()) {
                if (last_ref->second.find("next") != last_ref->second.end()) {
                    auto& next_ref = last_ref->second["next"];
                    if (next_ref.find("type") != next_ref.end() && next_ref["type"] != "options") {
                        next_ref["target"] = dialog_associate_key_[line_index_];
                        if (next_ref.find("type") == next_ref.end()) {
                            next_ref["type"] = "default";
                        }
                    }
                    else {
                        next_ref = {
                            {"target", dialog_associate_key_[line_index_]},
                            {"type", "default"}
                        };
                    }
                }
                else {
                    last_ref->second["next"] = {
                        {"target", dialog_associate_key_[line_index_]},
                        {"type", "default"}
                    };
                }
            }
            else {
                terminated("KeyError: " + previous_);
            }
        }
        else {
            current_data_.previous_ = nullptr;
        }

        if (!accumulated_comments_.empty()) {
            current_data_.comments_ = accumulated_comments_;
            accumulated_comments_.clear();
        }

        previous_ = dialog_associate_key_[line_index_];
        line_index_ += current_data_.contents_.size();
        output_[section_][previous_] = current_data_.to_map();
        current_data_.comments_.clear();
        }
        else {
            terminated("Invalid code or content!");
        }

        // Move to the next line
        ++line_index_;
    }
}
*/

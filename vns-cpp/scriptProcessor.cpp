#include <vector>
#include <cassert>
#include <stdexcept>
#include <fstream>
#include "functions.hpp"
#include "scriptProcessor.hpp"
#include "naming.hpp"

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
    throw std::runtime_error("File \"" + path_.string() + "\", failed to compile due to: " + reason);
}

[[noreturn]] void ScriptProcessor::terminated(const std::string &reason, const size_t &line_index) const
{
    throw std::runtime_error(
            "File \"" + path_.string() + "\", line " + std::to_string(line_index + 1) + "\n  " + lines_[line_index] +
            "\nFail to compile due to: " + reason);
}

DialogueSectionsDataType ScriptProcessor::get_output() const
{
    return output_.to_map();
}

nlohmann::json ScriptProcessor::get_output_as_json() const
{
    return output_.to_json();
}


void ScriptProcessor::process(const std::filesystem::path &path)
{
    path_ = path;
    size_t current_index = 0;

    //make sure file exists
    if (!exists(path_))
    {
        terminated("File does not exist!");
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
            terminated("File is occupied!");
        }
    }

    // make sure file is not empty
    if (lines_.empty())
    {
        terminated("Cannot convert an empty script file!");
    }

    std::string last_label;

    for (size_t index = 0; index < lines_.size(); ++index)
    {
        lines_[index] = trim(lines_[index].substr(0, lines_[index].find(COMMENT_PREFIX)));

        if (lines_[index].starts_with(TAG_STARTS))
        {
            if (auto tag = extract_tag(lines_[index]); tag == "label")
            {
                if (!last_label.empty())
                {
                    terminated("This label is overwriting the previous one", index);
                }

                last_label = extract_parameter(lines_[index]);
                if (RESERVED_WORDS.contains(last_label))
                {
                    terminated("You cannot use reserved word '" + last_label + "' as a label", index);
                }
            } else if (tag == "section")
            {
                current_index = 0;
            }
        } else if (lines_[index].ends_with(':'))
        {
            dialog_associate_key_[index] =
                    current_index == 0 ? "head" : last_label.empty() ? (current_index < 10 ? "~0" + std::to_string(
                            current_index) : "~" + std::to_string(current_index)) : last_label;
            last_label.clear();
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

void ScriptProcessor::convert(const size_t starting_index)
{
    size_t line_index = starting_index;

    while (line_index < lines_.size())
    {
        // obtain current line as a pointer
        const std::string &current_line = lines_[line_index];
        // Skip empty lines or lines starting with #
        if (current_line.empty() || lines_[line_index].starts_with(COMMENT_PREFIX))
        {
            // Do nothing
        } else if (current_line.starts_with(NOTE_PREFIX))
        {
            // Accumulate notes
            current_data_.notes.push_back(current_line.substr(NOTE_PREFIX.length() + 1));
        } else if (current_line.starts_with(TAG_STARTS))
        {
            std::string tag = extract_tag(current_line);

            // check if the tag is an alternative of another tag
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
                    terminated("Chapter id cannot be None!", line_index);
                }
            } else if (tag == "language")
            {
                lang_ = extract_string(current_line);
            } else if (tag == "section")
            {
                if (!previous_.empty())
                {
                    output_.get_dialogue(section_, previous_).set_next();
                }
                // if section has no content, then remove head
                if (output_.contains_section(section_) && output_.get_dialogues(section_).size() == 1 &&
                    output_.get_dialogue(section_, "head").to_json().empty())
                {
                    output_.set_dialogues(section_, {});
                }
                section_ = extract_string(current_line);
                output_.set_dialogues(section_, {});
                DialogueDataType dialogue_data;
                output_.set_dialogue(section_, "head", dialogue_data);
                current_data_ = Dialogue({}, "head");
                previous_.clear();
            } else if (tag == "end")
            {
                assert(!previous_.empty());
                output_.get_dialogue(section_, previous_).set_next();
            } else if (tag == "scene")
            {
                assert(!previous_.empty());
                if (output_.get_dialogue(section_, previous_).next.has_multi_targets())
                {
                    output_.get_dialogue(section_, previous_).set_next("scene", output_.get_dialogue(section_,
                                                                                                     previous_).next.get_targets());
                } else
                {
                    output_.get_dialogue(section_, previous_).set_next("scene", output_.get_dialogue(section_,
                                                                                                     previous_).next.get_target());
                }
                current_data_.background_image = extract_parameter(current_line);
                blocked_ = true;
            } else if (tag == "block")
            {
                blocked_ = true;
            } else if (tag == "option")
            {
                if (current_data_.contents.empty())
                {
                    terminated("Invalid option syntax: '->' cannot be found!", line_index);
                }
                // get current targets
                MultiTargetsType current_targets;
                if (output_.get_dialogue(section_, previous_).next.has_type("options"))
                {
                    current_targets = output_.get_dialogue(section_, previous_).next.get_targets();
                }
                // get value string
                auto src_to_target = extract_string(current_line);
                // push text and id map
                current_targets.push_back({{"text", trim(src_to_target.substr(0, src_to_target.find("->")))},
                                           {"id",   ensure_not_null(
                                                   trim(src_to_target.substr(src_to_target.find("->") + 2)))}});
                // update next
                output_.get_dialogue(section_, previous_).set_next("options", current_targets);
            }
                // Placeholder, no action needed for "label" tag
            else if (tag == "label")
            {
            } else
            {
                terminated("Invalid tag " + tag, line_index);
            }
        } else if (current_line.ends_with(':'))
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

            for (size_t sub_index = line_index + 1; sub_index < lines_.size(); ++sub_index)
            {
                if (!lines_[sub_index].starts_with('-'))
                {
                    break;
                }
                current_data_.contents.push_back(trim(lines_[sub_index].substr(1)));
            }

            if (section_.empty())
            {
                terminated("You have to specify section before script", line_index);
            }
            if (!output_.contains_section(section_))
            {
                output_.set_current_section_dialogues({});
            }

            if (!previous_.empty())
            {
                if (!blocked_)
                {
                    current_data_.previous = previous_;
                } else
                {
                    current_data_.previous.clear();
                    blocked_ = false;
                }

                if (output_.contains_dialogue(section_, previous_))
                {
                    if (output_.get_dialogue(section_, previous_).has_next())
                    {
                        if (!output_.get_dialogue(section_, previous_).next.has_type("options"))
                        {
                            output_.get_dialogue(section_, previous_).set_next(
                                    output_.get_dialogue(section_, previous_).next.get_type(),
                                    dialog_associate_key_[line_index]);
                        }
                    } else
                    {
                        output_.get_dialogue(section_, previous_).set_next(
                                output_.get_dialogue(section_, previous_).next.get_type(),
                                dialog_associate_key_[line_index]);
                    }
                } else
                {
                    terminated("KeyError: " + previous_, line_index);
                }
            } else
            {
                current_data_.previous.clear();
            }

            previous_ = dialog_associate_key_[line_index];
            line_index += current_data_.contents.size();
            DialogueDataType current_data_map = current_data_.to_map();
            output_.set_dialogue(section_, previous_, current_data_map);
            current_data_.notes.clear();
            current_data_.events.clear();
        } else if (const size_t eql_location = current_line.find('='); eql_location != std::string::npos)
        {
            // get the operation, set (a=1), add (a+=1), and so on, which is why eql_location matters
            const std::string variable_action = operations_.contains(current_line[eql_location - 1]) ? operations_.at(
                    current_line[eql_location - 1]) : "set";
            // get the name of the variable
            const std::string variable_name = trim(
                    current_line.substr(0, variable_action != "set" ? eql_location - 1 : eql_location));
            // get the value of the variable
            const std::string variable_value = trim(current_line.substr(eql_location + 1));
            // make sure variable_value is not empty
            if (variable_value.empty())
            {
                throw std::runtime_error(variable_name + " has empty value!");
            }
            // create a variant for holding
            EventValueType event_value;
            // if variable value is true (boolean)
            if (variable_value == "true")
            {
                if (variable_action != "set")
                {
                    terminated("You can only set a boolean variable!");
                }
                event_value = true;
            }
                // if variable value is false (boolean)
            else if (variable_value == "false")
            {
                if (variable_action != "set")
                {
                    terminated("You can only set a boolean variable!");
                }
                event_value = false;
            }
                // if variable value is string
            else if (variable_value.starts_with('"'))
            {
                if (!variable_value.ends_with('"'))
                {
                    terminated("Possible missing close quotation mark for string", line_index);
                } else if (variable_action != "set")
                {
                    terminated("You can only set a string variable!");
                }
                event_value = variable_value;
            } else
            {
                size_t pos;
                // if variable value is float number
                if (variable_value.find('.') != std::string::npos)
                {
                    try
                    {
                        event_value = std::stof(variable_value, &pos);
                    } catch (std::invalid_argument &)
                    {
                        pos = 0;
                    }
                }
                    // if variable value is int number?
                else
                {
                    try
                    {
                        event_value = std::stoi(variable_value, &pos);
                    } catch (std::invalid_argument &)
                    {
                        pos = 0;
                    }
                }
                // If not all characters were processed, then assume it is a number / math expression
                if (pos != variable_value.size())
                {
                    event_value = variable_value;
                }
            }
            current_data_.events.emplace_back(variable_action, variable_name, event_value);
        } else if (current_line.ends_with("++"))
        {
            current_data_.events.emplace_back("add", trim(current_line.substr(0, current_line.size() - 2)), 1);
        } else if (current_line.ends_with("--"))
        {
            current_data_.events.emplace_back("subtract", trim(current_line.substr(0, current_line.size() - 2)), 1);
        } else
        {
            terminated("Invalid code or content!", line_index);
        }
        // Move to the next line
        line_index++;
    }
}

#include <vector>
#include <stdexcept>
#include <fstream>
#include "functions.hpp"
#include "scriptProcessor.hpp"
#include "naming.hpp"
#include "operation.hpp"
#include "version.hpp"

std::string ScriptProcessor::get_id() const
{
    return id_;
}

std::string ScriptProcessor::get_language() const
{
    return language_;
}

std::string ScriptProcessor::ensure_not_null(const std::string& text)
{
    return iequals(text, "null") || iequals(text, "none") ? std::string() : text;
}

std::string ScriptProcessor::extract_parameter(const std::string& text)
{
    return ensure_not_null(extract_string(text));
}

std::string_view ScriptProcessor::extract_tag(const std::string& text)
{
    const std::string_view tag = {RANGE(text, text.find(TAG_STARTS) + 1, text.find(TAG_ENDS))};
    // check if the tag is an alternative of another tag
    return tags::alternatives.contains(tag) ? tags::alternatives.at(tag) : tag;
}

std::string ScriptProcessor::extract_string(const std::string& text)
{
    return trim(text.substr(text.find(TAG_ENDS) + 1));
}

[[noreturn]] void ScriptProcessor::terminated(const std::string& reason) const
{
    std::stringstream errMsg;
    if (path_.empty())
    {
        errMsg << "Failed";
    }
    else
    {
        errMsg << "File " << '"' << path_ << '"' << ", failed";
    }
    errMsg << " to compile due to: " << reason;
    throw std::runtime_error(errMsg.str());
}

[[noreturn]] void ScriptProcessor::terminated(const std::string& reason, const size_t& line_index) const
{
    std::stringstream errMsg;
    if (path_.empty())
    {
        errMsg << "Line";
    }
    else
    {
        errMsg << "File " << '"' << path_ << '"' << ", line";
    }
    errMsg << ' ' << std::to_string(line_index + 1) << "\n  >>|" << lines_[line_index] << "|\nFail to compile due to: "
        << reason;
    throw std::runtime_error(errMsg.str());
}

[[noreturn]] void
ScriptProcessor::terminated(const std::string& reason, const size_t& line_index, const std::string_view& tag) const
{
    std::stringstream errMsg;
    if (path_.empty())
    {
        errMsg << "Line";
    }
    else
    {
        errMsg << "File " << '"' << path_ << '"' << ", line";
    }
    errMsg << ' ' << std::to_string(line_index + 1) << "\n  >>|" << lines_[line_index] << "|\nFail to compile due to "
        << reason << ':' << ' ' << tag;
    throw std::runtime_error(errMsg.str());
}

DialoguesManager ScriptProcessor::get_output() const
{
    return output_;
}

void ScriptProcessor::process(const std::string& raw_data)
{
    process(split(raw_data, '\n'));
}

void ScriptProcessor::process(const std::vector<std::string>& lines)
{
    lines_ = lines;
    continue_process();
}

void ScriptProcessor::process(const std::filesystem::path& path)
{
    path_ = path;

    //make sure the file exists
    if (!exists(path_))
    {
        terminated("File does not exist!");
    }

    // read file
    if (path_.extension() == SCRIPTS_FILE_EXTENSION)
    {
        try
        {
            load_file_as_lines(path_, lines_);
        }
        catch ([[maybe_unused]] const std::exception& e)
        {
            terminated("File is occupied!");
        }
    }
    continue_process();
}

void ScriptProcessor::continue_process()
{
    // make sure the file is not empty
    if (lines_.empty())
    {
        terminated("Cannot convert an empty script file!");
    }

    std::string last_label;

    size_t current_index = 0;

    // lines clean up
    for (auto& line : lines_)
        line = trim(line.substr(0, line.find(COMMENT_PREFIX)));

    // remove all empty lines or lines starting with # (comments)
    lines_.erase(std::ranges::remove_if(lines_, [](const std::string& line)
    {
        return line.empty() || line.starts_with(COMMENT_PREFIX);
    }).begin(), lines_.end());

    // pre-process all the lines
    for (size_t i = 0; i < lines_.size(); ++i)
    {
        // get the current line as a pointer
        if (const std::string& current_line = lines_[i]; current_line.starts_with(TAG_STARTS))
        {
            if (const std::string_view tag = extract_tag(current_line); tag == tags::label)
            {
                if (!last_label.empty())
                {
                    terminated("This label is overwriting the previous one", i);
                }

                last_label = extract_parameter(current_line);
                if (RESERVED_WORDS.contains(last_label))
                {
                    terminated("You cannot use reserved word '" + last_label + "' as a label", i);
                }
            }
            else if (tag == tags::vns)
            {
                // make sure a match exists, if not, then the given value is invalid
                std::smatch match;
                if (const std::string version_info = extract_parameter(current_line); !std::regex_match(
                    version_info, match, vns_version_pattern))
                    terminated("Invalid tag value", i, tag);
                // check if the given script version is compatible with the compiler
                bool is_script_compatible;
                try
                {
                    is_script_compatible = is_version_compatible(match[1].str(), std::stoul(match[2].str()),
                                                                 std::stoul(match[3].str()));
                }
                // if there is an error, likely due to invalid comparator, then throw it with line index
                catch (const std::runtime_error& e)
                {
                    terminated(e.what(), i, tag);
                }
                // throw error if it is not compatible
                if (!is_script_compatible)
                {
                    terminated("Version incompatible", i);
                }
            }
            else if (tag == tags::id)
            {
                id_ = extract_parameter(current_line);
                if (id_.empty())
                    terminated("Chapter id cannot be None!", i);
            }
            else if (tag == tags::language)
            {
                language_ = extract_string(current_line);
            }
        }
        else if (current_line.ends_with(':'))
        {
            dialog_associate_key_[i] = current_index == 0
                                           ? "head"
                                           : last_label.empty()
                                           ? (current_index < 10
                                                  ? "~0" + std::to_string(current_index)
                                                  : "~" + std::to_string(current_index))
                                           : last_label;
            last_label.clear();
            ++current_index;
        }
    }

    // making sure essential instances are initialized correctly
    if (id_.empty())
    {
        terminated("You have to set a id!");
    }
    if (language_.empty())
    {
        terminated("You have to set the language!");
    }

    convert(0);
    lines_.clear();
}

void ScriptProcessor::convert(const size_t starting_index)
{
    size_t line_index = starting_index;

    while (line_index < lines_.size())
    {
        // obtain the current line as a pointer
        if (const std::string& current_line = lines_[line_index]; current_line.starts_with(NOTE_PREFIX))
        {
            // Accumulate notes
            current_data_.notes.push_back(current_line.substr(NOTE_PREFIX.length() + 1));
        }
        else if (current_line.starts_with(TAG_STARTS))
        {
            if (const std::string_view tag = extract_tag(current_line); tag == tags::background_image)
            {
                current_data_.background_image = extract_parameter(current_line);
            }
            else if (tag == tags::background_music)
            {
                current_data_.background_music = extract_parameter(current_line);
            }
            else if (tag == tags::show)
            {
                for (const std::string& name : split(extract_string(current_line)))
                {
                    current_data_.character_images.push_back(name);
                }
            }
            else if (tag == tags::hide)
            {
                for (const std::string& name : split(extract_string(current_line)))
                {
                    if (name == "*")
                    {
                        current_data_.character_images.clear();
                        break;
                    }
                    std::erase_if(current_data_.character_images, [&](const std::string& n)
                    {
                        return Naming(n).equal(name);
                    });
                }
            }
            else if (tag == tags::display)
            {
                current_data_.character_images.clear();
                for (const std::string& name : split(extract_string(current_line)))
                {
                    current_data_.character_images.push_back(name);
                }
            }
            else if (tag == tags::end)
            {
                if (!previous_.empty())
                {
                    output_.get_dialogue(previous_).remove_next();
                    previous_.clear();
                }
            }
            else if (tag == tags::scene)
            {
                if (previous_.empty())
                {
                    terminated("Cannot use scene tag when there is not previous dialogue.", line_index);
                }
                if (Dialogue& previous_dialogue = output_.get_dialogue(previous_); previous_dialogue.next.
                    has_multi_targets())
                {
                    previous_dialogue.set_next("scene", previous_dialogue.next.get_targets());
                }
                else
                {
                    previous_dialogue.set_next("scene", previous_dialogue.next.get_target());
                }
                current_data_.background_image = extract_parameter(current_line);
                blocked_ = true;
            }
            else if (tag == tags::block)
            {
                blocked_ = true;
            }
            else if (tag == tags::option)
            {
                if (current_data_.contents.empty())
                {
                    terminated("Invalid option syntax: '->' cannot be found!", line_index);
                }
                // get current targets
                multi_targets_t current_targets;
                if (output_.get_dialogue(previous_).next.has_type("options"))
                {
                    current_targets = output_.get_dialogue(previous_).next.get_targets();
                }
                // get value string
                const std::string src_to_target = extract_string(current_line);
                // push text and id map
                const std::string option_points_to = ensure_not_null(
                    trim(src_to_target.substr(src_to_target.find("->") + 2)));
                current_targets.push_back({
                    {"text", trim(src_to_target.substr(0, src_to_target.find("->")))},
                    {"id", option_points_to}
                });
                branches_[option_points_to] = previous_;
                // update next
                output_.get_dialogue(previous_).set_next("options", current_targets);
            }
            else if (tag == tags::jump || tag == tags::jump_)
            {
                // cannot jump when the previous dialogue does not exist
                if (previous_.empty())
                {
                    terminated("Cannot use jump tag when there is not previous dialogue.", line_index);
                }
                // cannot jump when the previous dialogue has multiple targets
                if (output_.get_dialogue(previous_).next.has_multi_targets())
                {
                    terminated("Cannot use jump tag when previous dialogue already has multiple targets.", line_index);
                }
                // update previous dialogue's next
                const std::string jump_target = extract_parameter(current_line);
                output_.get_dialogue(previous_).set_next(
                    output_.get_dialogue(previous_).next.get_type(), jump_target);
                // if the tag is jump not jump_, then we need to overwrite jump_target's previous
                if (tag == tags::jump)
                {
                    // if jump_target already exist, then update jump_target's previous
                    if (output_.contains_dialogue(jump_target))
                    {
                        output_.get_dialogue(jump_target).previous = previous_;
                    }
                    // write branch info into the lookup table for future reference
                    else
                    {
                        branches_[jump_target] = previous_;
                    }
                }
                // remove prev
                previous_.clear();
            }
            else if (tag == tags::sound_effect)
            {
                current_data_.sound_effects.clear();
                for (const std::string& name : split(extract_string(current_line)))
                {
                    current_data_.sound_effects.push_back(name);
                }
            }
            else if (!preprocessed_tags.contains(tag))
            {
                terminated("invalid tag", line_index, tag);
            }
        }
        else if (current_line.ends_with(':'))
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
            for (std::string& character_image : current_data_.character_images)
            {
                Naming name_data(character_image);
                if (narrator_possible_images.contains(name_data.get_name()))
                {
                    name_data.erase_tag("silent");
                }
                else
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

            // update previous_ if it is supposed to be part of the branching operation
            if (branches_.contains(dialog_associate_key_[line_index]))
            {
                previous_ = branches_.at(dialog_associate_key_[line_index]);
            }

            // update current_data_ accordingly
            if (previous_.empty())
            {
                current_data_.previous.clear();
                blocked_ = false;
            }
            else
            {
                if (blocked_)
                {
                    current_data_.previous.clear();
                    blocked_ = false;
                }
                else
                {
                    current_data_.previous = previous_;
                }

                if (output_.contains_dialogue(previous_))
                {
                    if (output_.get_dialogue(previous_).has_next())
                    {
                        if (!output_.get_dialogue(previous_).next.has_type("options"))
                        {
                            output_.get_dialogue(previous_).set_next(
                                output_.get_dialogue(previous_).next.get_type(),
                                dialog_associate_key_[line_index]);
                        }
                    }
                    else
                    {
                        output_.get_dialogue(previous_).set_next(
                            output_.get_dialogue(previous_).next.get_type(),
                            dialog_associate_key_[line_index]);
                    }
                }
                else
                {
                    terminated("KeyError: " + previous_, line_index);
                }
            }

            previous_ = dialog_associate_key_[line_index];
            line_index += current_data_.contents.size();
            output_.set_dialogue(previous_, current_data_.to_map());
            current_data_.notes.clear();
            current_data_.events.clear();
            current_data_.sound_effects.clear();
        }
        else if (const size_t eql_location = current_line.find('='); eql_location != std::string::npos)
        {
            // get the operation, set (a=1), add (a+=1), and so on, which is why eql_location matters
            const std::string_view variable_action = operation::has(current_line[eql_location - 1])
                                                         ? operation::get(current_line[eql_location - 1])
                                                         : operation::set;
            // get the name of the variable
            const std::string variable_name = trim(
                current_line.substr(0, variable_action != operation::set ? eql_location - 1 : eql_location));
            // reject global variables (VEP 6)
            if (variable_name.starts_with('@'))
            {
                terminated(
                    "Global variables (@) are removed in VNS 3.0. Use local variables (no prefix) or persistent variables (&) instead",
                    line_index);
            }
            // get the value of the variable
            const std::string variable_value = trim(current_line.substr(eql_location + 1));
            // make sure variable_value is not empty
            if (variable_value.empty())
            {
                throw std::runtime_error(variable_name + " has empty value!");
            }
            // create a variant for holding
            event_data_t event_value;
            // if the variable value is true (boolean)
            if (variable_value == "true")
            {
                if (variable_action != operation::set)
                {
                    terminated("You can only set a boolean variable!");
                }
                event_value = true;
            }
            // if variable value is false (boolean)
            else if (variable_value == "false")
            {
                if (variable_action != operation::set)
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
                }
                if (variable_action != operation::set)
                {
                    terminated("You can only set a string variable!");
                }
                event_value = variable_value;
            }
            else
            {
                size_t pos;
                // if variable value is float number
                if (variable_value.find('.') != std::string::npos)
                {
                    try
                    {
                        event_value = std::stof(variable_value, &pos);
                    }
                    catch (std::invalid_argument&)
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
                    }
                    catch (std::invalid_argument&)
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
        }
        else if (current_line.ends_with("++"))
        {
            const std::string variable_name = trim(current_line.substr(0, current_line.size() - 2));
            // reject global variables (VEP 6)
            if (variable_name.starts_with('@'))
            {
                terminated(
                    "Global variables (@) are removed in VNS 3.0. Use local variables (no prefix) or persistent variables (&) instead",
                    line_index);
            }
            current_data_.events.emplace_back(operation::add, variable_name, 1);
        }
        else if (current_line.ends_with("--"))
        {
            const std::string variable_name = trim(current_line.substr(0, current_line.size() - 2));
            // reject global variables (VEP 6)
            if (variable_name.starts_with('@'))
            {
                terminated(
                    "Global variables (@) are removed in VNS 3.0. Use local variables (no prefix) or persistent variables (&) instead",
                    line_index);
            }
            current_data_.events.emplace_back(operation::subtract, variable_name, 1);
        }
        else
        {
            terminated("Invalid code or content!", line_index);
        }
        // Move to the next line
        ++line_index;
    }
}
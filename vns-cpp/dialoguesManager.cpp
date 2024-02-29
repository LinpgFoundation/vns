#include "dialoguesManager.hpp"
#include "scriptProcessor.hpp"

// Getter for previous dialogue of current dialogue
Dialogue *DialoguesManager::get_previous()
{
    return &dialog_data_[section_][get_current()->previous];
}

// Getter for current dialogue
Dialogue *DialoguesManager::get_current()
{
    return &dialog_data_[section_][current_dialog_id_];
}

// Getter for last selected dialogue
Dialogue *DialoguesManager::get_last()
{
    return &dialog_data_[section_][last_dialog_id_];
}

// load dialogue data from vns file
void DialoguesManager::load(const std::filesystem::path &path)
{
    ScriptProcessor processor;
    processor.process(path);
    update(processor.get_output());
}

// Check if data is empty
bool DialoguesManager::empty() const
{
    return dialog_data_.empty();
}

// Clear data
void DialoguesManager::clear()
{
    dialog_data_.clear();
}

// Update data
void DialoguesManager::update(const DialogueSectionsDataType &data)
{
    for (const auto &[section_name, section_dialogues]: data)
    {
        set_dialogues(section_name, section_dialogues);
    }
    set_current_dialogue_id("head");
}

// Go to next dialogue
void DialoguesManager::next()
{
    set_current_dialogue_id(get_current()->next.has_single_target() ? get_current()->next.get_target()
                                                                    : get_current()->next.get_targets()[0]["id"]);
};

// Contains variable
bool DialoguesManager::contains_variable(const std::string &name) const
{
    if (name.starts_with('@'))
    {
        return global_variables_.contains(name);
    } else if (name.starts_with('%'))
    {
        return persistent_variables_.contains(name);
    } else
    {
        return local_variables_.at(section_).contains(name);
    }
}

// Get variable
EventValueType DialoguesManager::get_variable(const std::string &name) const
{
    if (name.starts_with('@'))
    {
        return global_variables_.at(name);
    } else if (name.starts_with('%'))
    {
        return persistent_variables_.at(name);
    } else
    {
        return local_variables_.at(section_).at(name);
    }
}

// Set variable
void DialoguesManager::set_variable(const std::string &name, const EventValueType &value)
{
    if (name.starts_with('@'))
    {
        global_variables_[name] = value;
    } else if (name.starts_with('%'))
    {
        persistent_variables_[name] = value;
    } else
    {
        local_variables_[section_][name] = value;
    }
}

// Get data
DialogueSectionsDataType DialoguesManager::to_map() const
{
    DialogueSectionsDataType data_map;
    for (const auto &[section_name, section_dialogues]: dialog_data_)
    {
        if (!section_dialogues.empty())
        {
            data_map[section_name] = {};
            for (const auto &[dialogue_id, the_dialogue]: section_dialogues)
            {
                data_map[section_name][dialogue_id] = the_dialogue.to_map();
            }
        }
    }
    return data_map;
}

// Get data as json
nlohmann::json DialoguesManager::to_json() const
{
    nlohmann::json data_map;
    for (const auto &[section_name, section_dialogues]: dialog_data_)
    {
        if (!section_dialogues.empty())
        {
            data_map[section_name] = {};
            for (const auto &[dialogue_id, the_dialogue]: section_dialogues)
            {
                data_map[section_name][dialogue_id] = the_dialogue.to_json();
            }
        }
    }
    return data_map;
}

// Get current dialogue id
std::string DialoguesManager::get_current_dialogue_id() const
{
    return current_dialog_id_;
}

// Set current dialogue id
void DialoguesManager::set_current_dialogue_id(const std::string &id)
{
    last_dialog_id_ = current_dialog_id_;
    current_dialog_id_ = id;
    // process events
    for (const Event &e: get_current()->events)
    {
        if (e.type == "set")
        {
            set_variable(e.target, e.value);
        } else
        {
            const EventValueType current_value = get_variable(e.target);
            if (e.type == "add")
            {
                if (std::holds_alternative<float>(current_value))
                {
                    if (std::holds_alternative<float>(e.value))
                    {
                        set_variable(e.target, std::get<float>(current_value) + std::get<float>(e.value));
                    } else if (std::holds_alternative<int>(e.value))
                    {
                        set_variable(e.target, std::get<float>(current_value) + std::get<int>(e.value));
                    } else
                    {
                        throw std::runtime_error("Unable to add a non-number to" + e.target);
                    }
                } else if (std::holds_alternative<int>(current_value))
                {
                    if (std::holds_alternative<float>(e.value))
                    {
                        set_variable(e.target, std::get<int>(current_value) + std::get<float>(e.value));
                    } else if (std::holds_alternative<int>(e.value))
                    {
                        set_variable(e.target, std::get<int>(current_value) + std::get<int>(e.value));
                    } else
                    {
                        throw std::runtime_error("Unable to add a non-number to" + e.target);
                    }
                } else
                {
                    throw std::runtime_error("Unable to add to" + e.target + " because it is not a number");
                }
            } else if (e.type == "subtract")
            {
                if (std::holds_alternative<float>(current_value))
                {
                    if (std::holds_alternative<float>(e.value))
                    {
                        set_variable(e.target, std::get<float>(current_value) - std::get<float>(e.value));
                    } else if (std::holds_alternative<int>(e.value))
                    {
                        set_variable(e.target, std::get<float>(current_value) - std::get<int>(e.value));
                    } else
                    {
                        throw std::runtime_error("Unable to subtract a non-number to" + e.target);
                    }
                } else if (std::holds_alternative<int>(current_value))
                {
                    if (std::holds_alternative<float>(e.value))
                    {
                        set_variable(e.target, std::get<int>(current_value) - std::get<float>(e.value));
                    } else if (std::holds_alternative<int>(e.value))
                    {
                        set_variable(e.target, std::get<int>(current_value) - std::get<int>(e.value));
                    } else
                    {
                        throw std::runtime_error("Unable to subtract a non-number to" + e.target);
                    }
                } else
                {
                    throw std::runtime_error("Unable to subtract to" + e.target + " because it is not a number");
                }
            } else if (e.type == "multiply")
            {
                if (std::holds_alternative<float>(current_value))
                {
                    if (std::holds_alternative<float>(e.value))
                    {
                        set_variable(e.target, std::get<float>(current_value) * std::get<float>(e.value));
                    } else if (std::holds_alternative<int>(e.value))
                    {
                        set_variable(e.target, std::get<float>(current_value) * std::get<int>(e.value));
                    } else
                    {
                        throw std::runtime_error("Unable to multiply a non-number to" + e.target);
                    }
                } else if (std::holds_alternative<int>(current_value))
                {
                    if (std::holds_alternative<float>(e.value))
                    {
                        set_variable(e.target, std::get<int>(current_value) * std::get<float>(e.value));
                    } else if (std::holds_alternative<int>(e.value))
                    {
                        set_variable(e.target, std::get<int>(current_value) * std::get<int>(e.value));
                    } else
                    {
                        throw std::runtime_error("Unable to multiply a non-number to" + e.target);
                    }
                } else
                {
                    throw std::runtime_error("Unable to multiply to" + e.target + " because it is not a number");
                }
            } else if (e.type == "divide")
            {
                if (std::holds_alternative<float>(current_value))
                {
                    if (std::holds_alternative<float>(e.value))
                    {
                        set_variable(e.target, std::get<float>(current_value) / std::get<float>(e.value));
                    } else if (std::holds_alternative<int>(e.value))
                    {
                        set_variable(e.target, std::get<float>(current_value) / std::get<int>(e.value));
                    } else
                    {
                        throw std::runtime_error("Unable to divide a non-number to" + e.target);
                    }
                } else if (std::holds_alternative<int>(current_value))
                {
                    if (std::holds_alternative<float>(e.value))
                    {
                        set_variable(e.target, std::get<int>(current_value) / std::get<float>(e.value));
                    } else if (std::holds_alternative<int>(e.value))
                    {
                        set_variable(e.target, std::get<int>(current_value) / std::get<int>(e.value));
                    } else
                    {
                        throw std::runtime_error("Unable to divide a non-number to" + e.target);
                    }
                } else
                {
                    throw std::runtime_error("Unable to divide to" + e.target + " because it is not a number");
                }
            }
        }
    }
}

// Get current section name
std::string DialoguesManager::get_section() const
{
    return section_;
}

// Get the names of all sections
std::unordered_set<std::string> DialoguesManager::get_sections() const
{
    auto kv = std::views::keys(dialog_data_);
    return std::unordered_set<std::string>{kv.begin(), kv.end()};
}

// Set current section name
void DialoguesManager::set_section(const std::string &section)
{
    section_ = section;
    current_dialog_id_ = "head";
    last_dialog_id_.clear();
}

// Does dialogue have given section name
bool DialoguesManager::contains_section(const std::string &section)
{
    return dialog_data_.contains(section);
}

// Remove section
void DialoguesManager::remove_section(const std::string &section)
{
    dialog_data_.erase(section);
}

// Get current section dialogue contents
std::unordered_map<std::string, Dialogue> &DialoguesManager::get_current_section_dialogues()
{
    return get_dialogues(section_);
}

// Get section dialogue contents by section name
std::unordered_map<std::string, Dialogue> &DialoguesManager::get_dialogues(const std::string &section)
{
    return dialog_data_.at(section);
}

// Set current section dialogue contents
void DialoguesManager::set_current_section_dialogues(const DialogueSectionDataType &data)
{
    set_dialogues(section_, data);
}

// Set section dialogue contents by section name
void DialoguesManager::set_dialogues(const std::string &section, const DialogueSectionDataType &data)
{
    // make sure dialog_data will have given section as a key
    if (!dialog_data_.contains(section))
    {
        dialog_data_[section] = {};
    }
    // loop through the data and init data as dialogue object(s)
    for (const auto &[dialogue_id, dialogue_data]: data)
    {
        dialog_data_[section][dialogue_id] = Dialogue(dialogue_data, dialogue_id);
    }
}

// Get dialogue data
Dialogue &DialoguesManager::get_dialogue(const std::string &section, const std::string &id)
{
    return get_dialogues(section)[id];
}

// Set current dialogue data
void DialoguesManager::set_current_dialogue(DialogueDataType &data)
{
    set_dialogue(section_, current_dialog_id_, data);
}

// Set dialogue data
void DialoguesManager::set_dialogue(const std::string &section, const std::string &id, DialogueDataType &data)
{
    get_dialogues(section)[id] = Dialogue(data, id);
}

// Does section contain given dialogue id
bool DialoguesManager::contains_dialogue(const std::string &section, const std::string &id)
{
    return get_dialogues(section).contains(id);
}

// Remove current dialogue
void DialoguesManager::remove_current_dialogue()
{
    remove_dialogue(section_, current_dialog_id_);
}

// Remove dialogue
void DialoguesManager::remove_dialogue(const std::string &section, const std::string &id)
{
    get_dialogues(section).erase(id);
    if (id == current_dialog_id_)
    {
        current_dialog_id_.clear();
    }
}

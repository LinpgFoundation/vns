#include "dialoguesManager.hpp"
#include "scriptProcessor.hpp"
#include "expressionParser.hpp"
#include "validator.hpp"

// Getter for previous dialogue of current dialogue
Dialogue *DialoguesManager::get_previous()
{
    return &dialog_data_.at(section_).at(get_current()->previous);
}

// Getter for current dialogue
Dialogue *DialoguesManager::get_current()
{
    return &dialog_data_.at(section_).at(current_dialog_id_);
}

// load dialogue data from vns file
void DialoguesManager::load(const std::filesystem::path &path)
{
    if (path.extension() == ScriptProcessor::SCRIPTS_FILE_EXTENSION)
    {
        ScriptProcessor processor;
        processor.process(path);
        update(processor.get_output().to_map());
    } else if (path.extension() == ".json")
    {
        nlohmann::json data = load_json(path);
        Validator vns_validator;
        vns_validator.ensure(data);
        update(data.at("dialogues"));
    } else
    {
        throw std::runtime_error("The format of given file is not supported: " + path.string());
    }
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
void DialoguesManager::update(const dialogue_content_t &data)
{
    // reset section_
    section_.clear();
    // make sure there is no empty section name
    if (data.contains(std::string()))
    {
        throw std::runtime_error("Section name cannot be an empty string!");
    }
    // init data to Dialogue object(s)
    for (const auto &[section_name, section_dialogues]: data)
    {
        // use first section name as current selected section
        if (section_.empty())
        {
            section_ = section_name;
        }
        set_dialogues(section_name, section_dialogues);
    }
    // set id to head
    set_current_dialogue_id("head");
}

// Update data
void DialoguesManager::update(const nlohmann::json &data)
{
    // reset section_
    section_.clear();
    // make sure there is no empty section name
    if (data.contains(std::string()))
    {
        throw std::runtime_error("Section name cannot be an empty string!");
    }
    // init data to Dialogue object(s)
    for (const auto &[section_name, section_dialogues]: data.items())
    {
        // use first section name as current selected section
        if (section_.empty())
        {
            section_ = section_name;
        }
        set_dialogues(section_name, section_dialogues);
    }
    // set id to head
    set_current_dialogue_id("head");
}

// Go to next dialogue
void DialoguesManager::next()
{
    set_current_dialogue_id(get_current()->next.has_single_target() ? get_current()->next.get_target()
                                                                    : get_current()->next.get_targets()[0].at("id"));
}

// Contains variable
bool DialoguesManager::contains_variable(const std::string &name) const
{
    if (name.starts_with('@'))
    {
        return global_variables_.contains(name);
    } else if (name.starts_with('&'))
    {
        return persistent_variables_.contains(name);
    } else
    {
        return local_variables_.contains(section_) && local_variables_.at(section_).contains(name);
    }
}

// Get variable
event_data_t DialoguesManager::get_variable(const std::string &name) const
{
    if (name.starts_with('@'))
    {
        return global_variables_.at(name);
    } else if (name.starts_with('&'))
    {
        return persistent_variables_.at(name);
    } else
    {
        return local_variables_.at(section_).at(name);
    }
}

// Set variable
void DialoguesManager::set_variable(const std::string &name, const event_data_t &value)
{
    if (name.starts_with('@'))
    {
        global_variables_[name] = value;
    } else if (name.starts_with('&'))
    {
        persistent_variables_[name] = value;
    } else
    {
        local_variables_[section_][name] = value;
    }
}

// Get data
dialogue_content_t DialoguesManager::to_map() const
{
    dialogue_content_t data_map;
    for (const auto &[section_name, section_dialogues]: dialog_data_)
    {
        if (!section_dialogues.empty())
        {
            data_map[section_name] = {};
            for (const auto &[dialogue_id, the_dialogue]: section_dialogues)
            {
                data_map.at(section_name)[dialogue_id] = the_dialogue.to_map();
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
                data_map.at(section_name)[dialogue_id] = the_dialogue.to_json();
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
    // make sure id exists in current section
    ensure_dialogue_existence(section_, id);
    // update dialogue id
    current_dialog_id_ = id;
    // process events
    for (const Event &e: get_current()->events)
    {
        if (e.type == "set")
        {
            if (std::holds_alternative<bool>(e.value))
            {
                set_variable(e.target, std::get<bool>(e.value));
            } else if (std::holds_alternative<int>(e.value))
            {
                set_variable(e.target, std::get<int>(e.value));
            } else if (std::holds_alternative<float>(e.value))
            {
                set_variable(e.target, std::get<float>(e.value));
            } else
            {
                const std::string value_str = std::get<std::string>(e.value);
                if (value_str.starts_with('"') && value_str.ends_with('"'))
                {
                    set_variable(e.target, value_str.substr(1, value_str.size() - 2));
                } else
                {
                    // get the result
                    const Number result = parse_expression(value_str);
                    // set the variable accordingly
                    result.is_int() ? set_variable(e.target, result.get_int()) : set_variable(e.target,
                                                                                              result.get_float());
                }
            }
        } else
        {
            const event_data_t current_value = get_variable(e.target);

            // make sure source variable is a number
            if (std::holds_alternative<bool>(current_value))
            {
                throw std::runtime_error("Unable to add to " + e.target + " because it is not a bool");
            } else if (std::holds_alternative<std::string>(current_value))
            {
                throw std::runtime_error("Unable to add to " + e.target + " because it is not a string");
            }
            // cast source variable as number
            Number n1 = std::holds_alternative<int>(current_value) ? Number(std::get<int>(current_value)) : Number(
                    std::get<float>(current_value));
            // make sure destination variable is a number
            if (std::holds_alternative<bool>(e.value))
            {
                throw std::runtime_error("Unable to add a bool to " + e.target);
            } else if (std::holds_alternative<std::string>(e.value))
            {
                if (const std::string value_str = std::get<std::string>(e.value); value_str.starts_with('"') ||
                                                                                  value_str.ends_with('"'))
                {
                    throw std::runtime_error("Unable to add a string to " + e.target);
                }
            }
            // cast destination variable as number
            const Number n2 = std::holds_alternative<int>(e.value) ? Number(std::get<int>(e.value))
                                                                   : (std::holds_alternative<float>(e.value) ? Number(
                            std::get<float>(e.value)) : Number(parse_expression(std::get<std::string>(e.value))));
            // get the result
            n1.operate(e.type, n2);
            // set the variable accordingly
            n1.is_int() ? set_variable(e.target, n1.get_int()) : set_variable(e.target, n1.get_float());
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
    set_current_dialogue_id("head");
}

// Does dialogue have given section name
bool DialoguesManager::contains_section(const std::string &section) const
{
    return dialog_data_.contains(section);
}

// Remove section
void DialoguesManager::remove_section(const std::string &section)
{
    // erase the section from dialog_data_
    dialog_data_.erase(section);
    // if current section did not get removed, then everything should be fine
    if (section != section_)
        return;
    // in the case where current section got remove
    // and result in dialog_data_ becoming empty
    if (dialog_data_.empty())
    {
        // force reset
        section_.clear();
        current_dialog_id_ = "head";
        return;
    }
    // if not, then use the first key as current section
    set_section(dialog_data_.begin()->first);
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
void DialoguesManager::set_current_section_dialogues(const dialogue_section_t &data)
{
    set_dialogues(section_, data);
}

// Set section dialogue contents by section name
void DialoguesManager::set_dialogues(const std::string &section, const dialogue_section_t &data)
{
    // make sure dialog_data will have given section as a key
    if (!contains_section(section))
    {
        dialog_data_[section] = {};
    }
    // loop through the data and init data as dialogue object(s)
    for (const auto &pair: data)
    {
        set_dialogue(section, pair.first, pair.second);
    }
    // if current_dialog_id_ no longer exists, then reset it to head
    if (section == section_ && !contains_dialogue(section_, current_dialog_id_))
    {
        set_current_dialogue_id("head");
    }
}

// Set section dialogue contents by section name
void DialoguesManager::set_dialogues(const std::string &section, const nlohmann::json &data)
{
    // make sure dialog_data will have given section as a key
    if (!contains_section(section))
    {
        dialog_data_[section] = {};
    }
    // loop through the data and init data as dialogue object(s)
    for (const auto &[dialogue_id, dialogue_json]: data.items())
    {
        set_dialogue(section, dialogue_id, dialogue_json);
    }
    // if current_dialog_id_ no longer exists, then reset it to head
    if (section == section_ && !contains_dialogue(section_, current_dialog_id_))
    {
        set_current_dialogue_id("head");
    }
}

// Get dialogue data
Dialogue &DialoguesManager::get_dialogue(const std::string &section, const std::string &id)
{
    ensure_dialogue_existence(section, id);
    return get_dialogues(section).at(id);
}

// Set current dialogue data
void DialoguesManager::set_current_dialogue(dialogue_data_t &data)
{
    set_dialogue(section_, current_dialog_id_, data);
}

// Set dialogue data
void DialoguesManager::set_dialogue(const std::string &section, const std::string &id, const dialogue_data_t &data)
{
    get_dialogues(section)[id] = Dialogue(id, data);
}

// Set dialogue data
void DialoguesManager::set_dialogue(const std::string &section, const std::string &id, const nlohmann::json &data)
{
    get_dialogues(section)[id] = Dialogue::from_json(id, data);
}

// Does section contain given dialogue id
bool DialoguesManager::contains_dialogue(const std::string &section, const std::string &id) const
{
    return dialog_data_.contains(section) && dialog_data_.at(section).contains(id);
}

// Make sure dialogue exists
void DialoguesManager::ensure_dialogue_existence(const std::string &section, const std::string &id) const
{
    if (!contains_dialogue(section, id))
    {
        std::stringstream msg;
        msg << "Dialogue id " << '"' << id << '"' << " does exist in section " << '"' << section << '"' << '!';
        throw std::runtime_error(msg.str());
    }
}

// Remove current dialogue
void DialoguesManager::remove_current_dialogue()
{
    remove_dialogue(section_, current_dialog_id_);
}

// Remove dialogue
void DialoguesManager::remove_dialogue(const std::string &section, const std::string id)
{
    // get current section dialogues map pointer
    std::unordered_map<std::string, Dialogue> &theDialogues = get_dialogues(section);
    // the dialogue that needs to remove
    const Dialogue &theOneToRemove = get_dialogue(section, id);
    // if removing head
    if (id == "head")
    {
        if (!theOneToRemove.has_next())
        {
            throw std::runtime_error("Cannot remove head when there is no next");
        } else if (theOneToRemove.next.has_multi_targets())
        {
            throw std::runtime_error("Cannot remove head when head.next has multiple targets.");
        }
        dialogue_data_t theNextDialogueData = get_dialogue(section, theOneToRemove.next.get_target()).to_map();
        // head will not have prev
        theNextDialogueData.erase("previous");
        // if current_dialog_id_ got removed, then set current_dialog_id_ back to head
        if (theOneToRemove.next.get_target() == current_dialog_id_)
        {
            set_current_dialogue_id("head");
        }
        // remove old next
        theDialogues.erase(theOneToRemove.next.get_target());
        // use next data as head data
        set_dialogue(section, id, theNextDialogueData);
        // fix new head's next(s)'s previous to head.id
        if (theOneToRemove.has_next())
        {
            if (theOneToRemove.next.has_single_target())
            {
                get_dialogue(section, theOneToRemove.next.get_target()).previous = id;
            } else
            {
                for (auto &t: theOneToRemove.next.get_targets())
                {
                    get_dialogue(section, t["id"]).previous = id;
                }
            }
        }
        // that is it
        return;
    }

    // if dialogue has only one next, the try to search all the dialogues,
    // and replace next with theOneToRemove.next
    if (theOneToRemove.next.has_single_target())
    {
        const std::string currNextTarget = theOneToRemove.next.get_target();
        for (auto &e: theDialogues)
        {
            // if dialogue does not have target, then do nothing
            if (!e.second.next.contains_target(id))
            {
                continue;
            }
            // if next has only one target, then simple replace it
            if (e.second.next.has_single_target())
            {
                e.second.next = DialogueNext(e.second.next.get_type(), currNextTarget);
                continue;
            }
            // if next has target as (more than) one of the targets, then need to find all and replace it/them
            auto theDialogTargets = e.second.next.get_targets();
            std::for_each(theDialogTargets.begin(), theDialogTargets.end(), [&id, currNextTarget](auto &p) {
                if (p.at("id") == id)
                {
                    p["id"] = currNextTarget;
                }
            });
            e.second.next = DialogueNext(e.second.next.get_type(), theDialogTargets);
        }
        // if next is not empty, then also need to update next.prev
        if (!currNextTarget.empty())
        {
            get_dialogue(section, currNextTarget).previous = theOneToRemove.previous;
        }
    }
    // if current_dialog_id_ got removed, then set current_dialog_id_ back to head
    if (section == section_ && id == current_dialog_id_)
    {
        set_current_dialogue_id(get_current()->previous.empty() ? "head" : get_current()->previous);
    }
    // remove dialogue from map
    theDialogues.erase(id);
}

// Parse a string expression
Number DialoguesManager::parse_expression(const std::string &expression) const
{
    ExpressionParser parser(expression, [this](const std::string &s) {
        return this->contains_variable(s);
    }, [this](const std::string &s) {
        return this->get_variable(s);
    });
    return parser.parse();
}
#include "dialoguesManager.hpp"
#include "scriptProcessor.hpp"
#include "expressionParser.hpp"

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
EventValueType DialoguesManager::get_variable(const std::string &name) const
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
void DialoguesManager::set_variable(const std::string &name, const EventValueType &value)
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
            const EventValueType current_value = get_variable(e.target);

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
    current_dialog_id_ = "head";
    last_dialog_id_.clear();
}

// Does dialogue have given section name
bool DialoguesManager::contains_section(const std::string &section) const
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
bool DialoguesManager::contains_dialogue(const std::string &section, const std::string &id) const
{
    return dialog_data_.contains(section) && dialog_data_.at(section).contains(id);
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
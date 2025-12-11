#include "dialoguesManager.hpp"
#include "scriptProcessor.hpp"
#include "expressionParser.hpp"
#include "validator.hpp"

// Getter for previous dialogue of current dialogue
Dialogue* DialoguesManager::get_previous()
{
    return get_current()->previous.empty() ? nullptr : &dialog_data_.at(get_current()->previous);
}

// Getter for current dialogue
Dialogue* DialoguesManager::get_current()
{
    return &dialog_data_.at(current_dialog_id_);
}

// load dialogue data from vns file
void DialoguesManager::load(const std::filesystem::path& path)
{
    if (path.extension() == ScriptProcessor::SCRIPTS_FILE_EXTENSION)
    {
        ScriptProcessor processor;
        processor.process(path);
        update(processor.get_output().to_map());
    }
    else if (path.extension() == ".json")
    {
        nlohmann::json data = load_json(path);
        Validator::ensure(data);
        update(data.at("dialogues"));
    }
    else
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
void DialoguesManager::update(const dialogues_t& data)
{
    // init data to Dialogue object(s)
    for (const auto& [dialogue_id, dialogue_data] : data)
    {
        set_dialogue(dialogue_id, dialogue_data);
    }
    // set id to head
    set_current_dialogue_id("head");
}

// Update data
void DialoguesManager::update(const nlohmann::json& data)
{
    // init data to Dialogue object(s)
    for (const auto& [dialogue_id, dialogue_json] : data.items())
    {
        set_dialogue(dialogue_id, dialogue_json);
    }
    // set id to head
    set_current_dialogue_id("head");
}

// Go to the previous dialogue
void DialoguesManager::previous()
{
    if (get_current()->has_previous())
        set_current_dialogue_id(get_current()->previous);
}

// Go to the next dialogue
void DialoguesManager::next()
{
    set_current_dialogue_id(get_current()->next.has_single_target()
                                ? get_current()->next.get_target()
                                : get_current()->next.get_targets()[0].at("id"));
}

// Contains variable
bool DialoguesManager::contains_variable(const std::string& name) const
{
    if (name.starts_with('&'))
    {
        return persistent_variables_.contains(name);
    }
    return local_variables_.contains(name);
}

// Get variable
event_data_t DialoguesManager::get_variable(const std::string& name) const
{
    if (name.starts_with('&'))
    {
        return persistent_variables_.at(name);
    }
    return local_variables_.at(name);
}

// Set variable
void DialoguesManager::set_variable(const std::string& name, const event_data_t& value)
{
    if (name.starts_with('&'))
    {
        persistent_variables_[name] = value;
    }
    else
    {
        local_variables_[name] = value;
    }
}

// Get data
dialogues_t DialoguesManager::to_map() const
{
    dialogues_t data_map;
    for (const auto& [dialogue_id, the_dialogue] : dialog_data_)
    {
        data_map[dialogue_id] = the_dialogue.to_map();
    }
    return data_map;
}

// Get data as JSON
nlohmann::json DialoguesManager::to_json() const
{
    nlohmann::json data_map;
    for (const auto& [dialogue_id, the_dialogue] : dialog_data_)
    {
        data_map[dialogue_id] = the_dialogue.to_json();
    }
    return data_map;
}

// Get current dialogue id
std::string DialoguesManager::get_current_dialogue_id() const
{
    return current_dialog_id_;
}

// Set current dialogue id
void DialoguesManager::set_current_dialogue_id(const std::string& id)
{
    // make sure id exists
    ensure_dialogue_existence(id);
    // update dialogue id
    current_dialog_id_ = id;
    // process events
    for (const Event& e : get_current()->events)
    {
        if (e.type == "set")
        {
            if (std::holds_alternative<bool>(e.value))
            {
                set_variable(e.target, std::get<bool>(e.value));
            }
            else if (std::holds_alternative<int>(e.value))
            {
                set_variable(e.target, std::get<int>(e.value));
            }
            else if (std::holds_alternative<float>(e.value))
            {
                set_variable(e.target, std::get<float>(e.value));
            }
            else
            {
                if (const std::string value_str = std::get<std::string>(e.value);
                    value_str.starts_with('"') && value_str.ends_with('"'))
                {
                    set_variable(e.target, value_str.substr(1, value_str.size() - 2));
                }
                else
                {
                    // get the result
                    const Number result = parse_expression(value_str);
                    // set the variable accordingly
                    result.is_int()
                        ? set_variable(e.target, result.get_int())
                        : set_variable(e.target, result.get_float());
                }
            }
        }
        else
        {
            const event_data_t current_value = get_variable(e.target);

            // make sure the source variable is a number
            if (std::holds_alternative<bool>(current_value))
            {
                throw std::runtime_error("Unable to add to " + e.target + " because it is not a bool");
            }
            if (std::holds_alternative<std::string>(current_value))
            {
                throw std::runtime_error("Unable to add to " + e.target + " because it is not a string");
            }
            // cast source variable as number
            Number n1 = std::holds_alternative<int>(current_value)
                            ? Number(std::get<int>(current_value))
                            : Number(std::get<float>(current_value));
            // make sure the destination variable is a number
            if (std::holds_alternative<bool>(e.value))
            {
                throw std::runtime_error("Unable to add a bool to " + e.target);
            }
            if (std::holds_alternative<std::string>(e.value))
            {
                if (const std::string value_str = std::get<std::string>(e.value); value_str.starts_with('"') ||
                    value_str.ends_with('"'))
                {
                    throw std::runtime_error("Unable to add a string to " + e.target);
                }
            }
            // cast destination variable as number
            const Number n2 = std::holds_alternative<int>(e.value)
                                  ? Number(std::get<int>(e.value))
                                  : std::holds_alternative<float>(e.value)
                                  ? Number(std::get<float>(e.value))
                                  : Number(parse_expression(std::get<std::string>(e.value)));
            // get the result
            n1.operate(e.type, n2);
            // set the variable accordingly
            n1.is_int() ? set_variable(e.target, n1.get_int()) : set_variable(e.target, n1.get_float());
        }
    }
}

// Get all dialogues
std::unordered_map<std::string, Dialogue>& DialoguesManager::get_dialogues()
{
    return dialog_data_;
}

// Set all dialogues
void DialoguesManager::set_dialogues(const dialogues_t& data)
{
    // loop through the data and init data as dialogue object(s)
    for (const auto& [dialogue_id, dialogue_data] : data)
    {
        set_dialogue(dialogue_id, dialogue_data);
    }
    // if current_dialog_id_ no longer exists, then reset it to head
    if (!contains_dialogue(current_dialog_id_))
    {
        current_dialog_id_ = "head";
    }
}

// Set all dialogues
void DialoguesManager::set_dialogues(const nlohmann::json& data)
{
    // loop through the data and init data as dialogue object(s)
    for (const auto& [dialogue_id, dialogue_json] : data.items())
    {
        set_dialogue(dialogue_id, dialogue_json);
    }
    // if current_dialog_id_ no longer exists, then reset it to head
    if (!contains_dialogue(current_dialog_id_))
    {
        current_dialog_id_ = "head";
    }
}

// Get dialogue data
Dialogue& DialoguesManager::get_dialogue(const std::string& id)
{
    ensure_dialogue_existence(id);
    return dialog_data_.at(id);
}

// Set current dialogue data
void DialoguesManager::set_current_dialogue(const dialogue_t& data)
{
    set_dialogue(current_dialog_id_, data);
}

// Set dialogue data
void DialoguesManager::set_dialogue(const std::string& id, const dialogue_t& data)
{
    dialog_data_[id] = Dialogue(id, data);
}

// Set dialogue data
void DialoguesManager::set_dialogue(const std::string& id, const nlohmann::json& data)
{
    dialog_data_[id] = Dialogue(id, data);
}

// Does dialogue with given id exist?
bool DialoguesManager::contains_dialogue(const std::string& id) const
{
    return dialog_data_.contains(id);
}

// Make sure dialogue exists
void DialoguesManager::ensure_dialogue_existence(const std::string& id) const
{
    if (!contains_dialogue(id))
    {
        std::stringstream msg;
        msg << "Dialogue id " << '"' << id << '"' << " does not exist!";
        throw std::runtime_error(msg.str());
    }
}

// Remove current dialogue
void DialoguesManager::remove_current_dialogue()
{
    remove_dialogue(current_dialog_id_);
}

// Remove dialogue
void DialoguesManager::remove_dialogue(const std::string id)
{
    // the dialogue that needs to remove
    const Dialogue& theOneToRemove = get_dialogue(id);
    // if removing head
    if (id == "head")
    {
        if (!theOneToRemove.has_next())
        {
            throw std::runtime_error("Cannot remove head when there is no next");
        }
        if (theOneToRemove.next.has_multi_targets())
        {
            throw std::runtime_error("Cannot remove head when head.next has multiple targets.");
        }
        dialogue_t theNextDialogueData = get_dialogue(theOneToRemove.next.get_target()).to_map();
        // head will not have prev
        theNextDialogueData.erase("previous");
        // if current_dialog_id_ got removed, then set current_dialog_id_ back to head
        if (theOneToRemove.next.get_target() == current_dialog_id_)
        {
            current_dialog_id_ = "head";
        }
        // remove old next
        dialog_data_.erase(theOneToRemove.next.get_target());
        // use next data as head data
        set_dialogue(id, theNextDialogueData);
        // fix new head's next(s)'s previous to head.id
        if (theOneToRemove.has_next())
        {
            if (theOneToRemove.next.has_single_target())
            {
                get_dialogue(theOneToRemove.next.get_target()).previous = id;
            }
            else
            {
                for (auto& t : theOneToRemove.next.get_targets())
                {
                    get_dialogue(t.at("id")).previous = id;
                }
            }
        }
        // that is it
        return;
    }

    // if dialogue has only one next, then try to search all the dialogues and replace the next with theOneToRemove.next
    if (theOneToRemove.next.has_single_target())
    {
        const std::string currNextTarget = theOneToRemove.next.get_target();
        for (auto& val : dialog_data_ | std::views::values)
        {
            // if dialogue does not have a target, then do nothing
            if (!val.next.contains_target(id))
            {
                continue;
            }
            // if next has only one target, then simple replace it
            if (val.next.has_single_target())
            {
                val.next = DialogueNext(val.next.get_type(), currNextTarget);
                continue;
            }
            // if next has a target as (more than) one of the targets, then need to find all and replace it/them
            auto theDialogTargets = val.next.get_targets();
            std::ranges::for_each(theDialogTargets, [&id, currNextTarget](auto& p)
            {
                if (p.at("id") == id)
                {
                    p["id"] = currNextTarget;
                }
            });
            val.next = DialogueNext(val.next.get_type(), theDialogTargets);
        }
        // if next is not empty, then also need to update next.prev
        if (!currNextTarget.empty())
        {
            get_dialogue(currNextTarget).previous = theOneToRemove.previous;
        }
    }
    // if current_dialog_id_ got removed, then set current_dialog_id_ back to head
    if (id == current_dialog_id_)
    {
        current_dialog_id_ = get_current()->previous.empty() ? "head" : get_current()->previous;
    }
    // remove dialogue from map
    dialog_data_.erase(id);
}

// Parse a string expression
Number DialoguesManager::parse_expression(const std::string& expression) const
{
    ExpressionParser parser(expression, [this](const std::string& s)
                            {
                                return this->contains_variable(s);
                            }, [this](const std::string& s)
                            {
                                return this->get_variable(s);
                            });
    return parser.parse();
}
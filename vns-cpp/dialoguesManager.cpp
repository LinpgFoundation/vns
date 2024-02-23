#include "dialoguesManager.hpp"
#include "scriptProcessor.hpp"

// If the pointer to the current dialogue data does not exist, set_data the pointer
void DialoguesManager::refresh_current()
{
    if (current_ == nullptr)
    {
        previous_ = current_;
        current_ = &dialog_data_[section_][current_dialog_id_];
    }
}

// Getters for previous dialogue
Dialogue *DialoguesManager::get_previous()
{
    refresh_current();
    return previous_;
}

// Getters for current dialogue
Dialogue *DialoguesManager::get_current()
{
    refresh_current();
    return current_;
}

// Getters for last dialogue
Dialogue *DialoguesManager::get_last()
{
    // if pointer does not exist, then use previous as last
    if (last_ == nullptr && !get_current()->previous.empty())
    {
        last_ = &dialog_data_[section_][get_current()->previous];
    }
    return last_;
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
    current_dialog_id_ = id;
    current_ = nullptr;
    last_ = nullptr;
    previous_ = nullptr;
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
    current_ = nullptr;
    last_ = nullptr;
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

// Get current dialogue data
Dialogue &DialoguesManager::get_current_dialogue()
{
    return get_dialogue(section_, current_dialog_id_);
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
    current_ = nullptr;
    remove_dialogue(section_, current_dialog_id_);
}

// Remove dialogue
void DialoguesManager::remove_dialogue(const std::string &section, const std::string &id)
{
    get_dialogues(section).erase(id);
}

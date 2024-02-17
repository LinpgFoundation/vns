#include "dialoguesManager.hpp"

// If the pointer to the current dialogue data does not exist, set_data the pointer
void DialoguesManager::refresh_current()
{
    if (current_ == nullptr)
    {
        previous_ = current_;
        current_ = new Dialogue(dialog_data_[section_][id_], id_);
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
        last_ = new Dialogue(dialog_data_[section_][get_current()->previous], get_current()->previous);
    }
    return last_;
}

// Save modifications to the current dialogue interface
void DialoguesManager::save()
{
    dialog_data_[section_][id_] = get_current()->to_map();
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
void DialoguesManager::set_data(const DialogueSectionsDataType &data)
{
    dialog_data_ = data;
    set_id("head");
}

// Get data
DialogueSectionsDataType DialoguesManager::get_data() const
{
    return dialog_data_;
}

// Get current dialogue id
std::string DialoguesManager::get_id() const
{
    return id_;
}

// Set current dialogue id
void DialoguesManager::set_id(const std::string &id)
{
    id_ = id;
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
DialogueSectionDataType &DialoguesManager::get_current_section_dialogues()
{
    return get_section_dialogues(section_);
}

// Get section dialogue contents by section name
DialogueSectionDataType &DialoguesManager::get_section_dialogues(const std::string &section)
{
    return dialog_data_.at(section);
}

// Set current section dialogue contents
void DialoguesManager::set_current_section_dialogues(const DialogueSectionDataType &data)
{
    set_section_dialogues(section_, data);
}

// Set section dialogue contents by section name
void DialoguesManager::set_section_dialogues(const std::string &section, const DialogueSectionDataType &data)
{
    dialog_data_[section] = data;
}

// Get current dialogue data
DialogueDataType &DialoguesManager::get_current_dialogue()
{
    return get_dialogue(section_, id_);
}

// Get dialogue data
DialogueDataType &DialoguesManager::get_dialogue(const std::string &section, const std::string &id)
{
    return get_section_dialogues(section)[id];
}

// Set current dialogue data
void DialoguesManager::set_current_dialogue(DialogueDataType &data)
{
    set_dialogue(section_, id_, data);
}

// Set dialogue data
void DialoguesManager::set_dialogue(const std::string &section, const std::string &id, DialogueDataType &data)
{
    get_section_dialogues(section)[id] = data;
}

// Does section contain given dialogue id
bool DialoguesManager::contains_dialogue(const std::string &section, const std::string &id)
{
    return get_section_dialogues(section).contains(id);
}

// Remove current dialogue
void DialoguesManager::remove_current_dialogue()
{
    current_ = nullptr;
    remove_dialogue(section_, id_);
}

// Remove dialogue
void DialoguesManager::remove_dialogue(const std::string &section, const std::string &id)
{
    get_section_dialogues(section).erase(id);
}

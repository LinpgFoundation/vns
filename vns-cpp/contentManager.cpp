#include "contentManager.hpp"

// If the pointer to the current dialog data does not exist, set_data the pointer
void ContentManager::refresh_current()
{
    if (current_ == nullptr)
    {
        previous_ = current_;
        current_ = new Content(dialog_data_[section_][id_], id_);
    }
}

// Getters for previous dialog
Content *ContentManager::get_previous()
{
    refresh_current();
    return previous_;
}

// Getters for current dialog
Content *ContentManager::get_current()
{
    refresh_current();
    return current_;
}

// Getters for last dialog
Content *ContentManager::get_last()
{
    // if pointer does not exist, then use previous as last
    if (last_ == nullptr && !get_current()->previous.empty())
    {
        last_ = new Content(dialog_data_[section_][get_current()->previous], get_current()->previous);
    }
    return last_;
}

// Save modifications to the current dialog interface
void ContentManager::save()
{
    dialog_data_[section_][id_] = get_current()->to_map();
}

// Check if data is empty
bool ContentManager::empty() const
{
    return dialog_data_.empty();
}

// Clear data
void ContentManager::clear()
{
    dialog_data_.clear();
}

// Update data
void ContentManager::set_data(const DialogueDataType &data)
{
    dialog_data_ = data;
    set_id("head");
}

// Get data
DialogueDataType ContentManager::get_data() const
{
    return dialog_data_;
}

// Get current id
std::string ContentManager::get_id() const
{
    return id_;
}

// Update current id
void ContentManager::set_id(const std::string &id)
{
    id_ = id;
    current_ = nullptr;
    last_ = nullptr;
    previous_ = nullptr;
}

// Get current section name
std::string ContentManager::get_section() const
{
    return section_;
}

// Set current section name
void ContentManager::set_section(const std::string &section)
{
    section_ = section;
    current_ = nullptr;
    last_ = nullptr;
}

// Does dialogue have given section name
bool ContentManager::contains_section(const std::string &section)
{
    return dialog_data_.contains(section);
}

// Remove section
void ContentManager::remove_section(const std::string &section)
{
    dialog_data_.erase(section);
}

// Get section content
SectionDataType &ContentManager::get_section_contents()
{
    return get_section_contents(section_);
}

SectionDataType &ContentManager::get_section_contents(const std::string &section)
{
    return dialog_data_.at(section);
}

// Set section content
void ContentManager::set_section_contents(const SectionDataType &data)
{
    set_section_contents(section_, data);
}

void ContentManager::set_section_contents(const std::string &section, const SectionDataType &data)
{
    dialog_data_[section] = data;
}

// Get content data
ContentDataType &ContentManager::get_content()
{
    return get_content(section_, id_);
}

ContentDataType &ContentManager::get_content(const std::string &section, const std::string &id)
{
    return get_section_contents(section)[id];
}

// Set content data
void ContentManager::set_content(ContentDataType &data)
{
    set_content(section_, id_, data);
}

void ContentManager::set_content(const std::string &section, const std::string &id, ContentDataType &data)
{
    get_section_contents(section)[id] = data;
}

// Does dialogue with given section contain given id
bool ContentManager::contains_content(const std::string &section, const std::string &id)
{
    return get_section_contents(section).contains(id);
}

// Remove dialog data
void ContentManager::remove_content()
{
    remove_content(section_, id_);
}

void ContentManager::remove_content(const std::string &section, const std::string &id)
{
    get_section_contents(section).erase(id);
}

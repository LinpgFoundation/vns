#ifndef CONTENT_MANAGER_HPP
#define CONTENT_MANAGER_HPP

#include "dialogue.hpp"
#include <unordered_set>

class DialoguesManager
{
public:
    // Getters for previous dialogue
    Dialogue *get_previous();

    // Getters for current dialogue
    Dialogue *get_current();

    // Getters for last dialogue
    Dialogue *get_last();

    // load dialogue data from vns file
    void load(const std::filesystem::path &);

    // Save modifications to the current dialogue interface
    void save();

    // Check if data is empty
    [[nodiscard]] bool empty() const;

    // Clear data
    void clear();

    // Update data
    void set_data(const DialogueSectionsDataType &);

    // Get data
    [[nodiscard]] DialogueSectionsDataType get_data() const;

    // Get current dialogue id
    [[nodiscard]] std::string get_id() const;

    // Set current dialogue id
    void set_id(const std::string &);

    // Get current section name
    [[nodiscard]] std::string get_section() const;

    // Get the names of all sections
    [[nodiscard]] std::unordered_set<std::string> get_sections() const;

    // Set current section name
    void set_section(const std::string &);

    // Does dialogue have given section name
    bool contains_section(const std::string &);

    // Remove section
    void remove_section(const std::string &);

    // Get current section dialogue contents
    [[nodiscard]] DialogueSectionDataType &get_current_section_dialogues();

    // Get section dialogue contents by section name
    [[nodiscard]] DialogueSectionDataType &get_section_dialogues(const std::string &);

    // Set current section dialogue contents
    void set_current_section_dialogues(const DialogueSectionDataType &);

    // Set section dialogue contents by section name
    void set_section_dialogues(const std::string &, const DialogueSectionDataType &);

    // Get current dialogue data
    [[nodiscard]] DialogueDataType &get_current_dialogue();

    // Get dialogue data
    [[nodiscard]] DialogueDataType &get_dialogue(const std::string &, const std::string &);

    // Set current dialogue data
    void set_current_dialogue(DialogueDataType &);

    // Set dialogue data
    void set_dialogue(const std::string &, const std::string &, DialogueDataType &);

    // Does section contain given dialogue id
    bool contains_dialogue(const std::string &, const std::string &);

    // Remove current dialogue
    void remove_current_dialogue();

    // Remove dialogue
    void remove_dialogue(const std::string &, const std::string &);

private:
    DialogueSectionsDataType dialog_data_;
    std::string section_;
    std::string id_ = "head";
    Dialogue *current_ = nullptr;
    Dialogue *last_ = nullptr;
    Dialogue *previous_ = nullptr;

    // If the pointer to the current dialogue data does not exist, set_data the pointer
    void refresh_current();
};

#endif

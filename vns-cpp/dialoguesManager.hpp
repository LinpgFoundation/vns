#ifndef CONTENT_MANAGER_HPP
#define CONTENT_MANAGER_HPP

#include "dialogue.hpp"
#include "number.hpp"
#include <unordered_set>

class DialoguesManager
{
public:
    // Getter for previous dialogue of current dialogue
    Dialogue *get_previous();

    // Getter for current dialogue
    Dialogue *get_current();

    // load dialogue data from vns file
    void load(const std::filesystem::path &);

    // Check if data is empty
    [[nodiscard]] bool empty() const;

    // Clear data
    void clear();

    // Update data
    void update(const dialogue_content_t &);

    // Update data
    void update(const nlohmann::json &);

    // Go to next dialogue
    void next();

    // Contains variable
    bool contains_variable(const std::string &) const;

    // Get variable
    event_data_t get_variable(const std::string &) const;

    // Get variable with Type
    template<typename T> T get_variable(const std::string &name) const
    {
        return std::get<T>(get_variable(name));
    }

    // Set variable
    void set_variable(const std::string &, const event_data_t &);

    // Get data
    [[nodiscard]] dialogue_content_t to_map() const;

    // Get data as json
    [[nodiscard]] nlohmann::json to_json() const;

    // Get current dialogue id
    [[nodiscard]] std::string get_current_dialogue_id() const;

    // Set current dialogue id
    void set_current_dialogue_id(const std::string &id);

    // Get current section name
    [[nodiscard]] std::string get_section() const;

    // Get the names of all sections
    [[nodiscard]] std::unordered_set<std::string> get_sections() const;

    // Set current section name
    void set_section(const std::string &);

    // Does dialogue have given section name
    bool contains_section(const std::string &) const;

    // Remove section
    void remove_section(const std::string &);

    // Get current section dialogue contents
    [[nodiscard]] std::unordered_map<std::string, Dialogue> &get_current_section_dialogues();

    // Get section dialogue contents by section name
    [[nodiscard]] std::unordered_map<std::string, Dialogue> &get_dialogues(const std::string &section);

    // Set current section dialogue contents
    void set_current_section_dialogues(const dialogue_section_t &);

    // Set section dialogue contents by section name
    void set_dialogues(const std::string &section, const dialogue_section_t &data);

    // Set section dialogue contents by section name
    void set_dialogues(const std::string &section, const nlohmann::json &data);

    // Get dialogue data
    [[nodiscard]] Dialogue &get_dialogue(const std::string &, const std::string &);

    // Set current dialogue data
    void set_current_dialogue(dialogue_data_t &);

    // Set dialogue data
    void set_dialogue(const std::string &, const std::string &, const dialogue_data_t &);

    // Set dialogue data
    void set_dialogue(const std::string &, const std::string &, const nlohmann::json &);

    // Does section contain given dialogue id
    bool contains_dialogue(const std::string &, const std::string &) const;

    // Remove current dialogue
    void remove_current_dialogue();

    // Remove dialogue
    void remove_dialogue(const std::string &, std::string);

private:
    std::unordered_map<std::string, std::unordered_map<std::string, Dialogue>> dialog_data_;
    std::unordered_map<std::string, std::unordered_map<std::string, event_data_t>> local_variables_;
    std::unordered_map<std::string, event_data_t> global_variables_;
    std::unordered_map<std::string, event_data_t> persistent_variables_;
    std::string section_;
    std::string current_dialog_id_ = "head";

    // Parse a string expression
    Number parse_expression(const std::string &) const;

    // Make sure dialogue exists
    void ensure_dialogue_existence(const std::string &, const std::string &) const;
};

#endif

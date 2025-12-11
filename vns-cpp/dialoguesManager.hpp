#ifndef CONTENT_MANAGER_HPP
#define CONTENT_MANAGER_HPP

#include "dialogue.hpp"
#include "number.hpp"

class DialoguesManager
{
public:
    // Getter for previous dialogue of current dialogue
    Dialogue* get_previous();

    // Getter for current dialogue
    Dialogue* get_current();

    // load dialogue data from vns file
    void load(const std::filesystem::path&);

    // Check if data is empty
    [[nodiscard]] bool empty() const;

    // Clear data
    void clear();

    // Update data
    void update(const dialogues_t&);

    // Update data
    void update(const nlohmann::json&);

    // Go to the previous dialogue
    void previous();

    // Go to the next dialogue
    void next();

    // Contains variable
    bool contains_variable(const std::string&) const;

    // Get variable
    event_data_t get_variable(const std::string&) const;

    // Get variable with Type
    template <typename T>
    T get_variable(const std::string& name) const
    {
        return std::get<T>(get_variable(name));
    }

    // Set variable
    void set_variable(const std::string&, const event_data_t&);

    // Get data
    [[nodiscard]] dialogues_t to_map() const;

    // Get data as JSON
    [[nodiscard]] nlohmann::json to_json() const;

    // Get current dialogue id
    [[nodiscard]] std::string get_current_dialogue_id() const;

    // Set current dialogue id
    void set_current_dialogue_id(const std::string&);

    // Get all dialogues
    [[nodiscard]] std::unordered_map<std::string, Dialogue>& get_dialogues();

    // Set all dialogues
    void set_dialogues(const dialogues_t&);

    // Set all dialogues
    void set_dialogues(const nlohmann::json&);

    // Get dialogue data
    [[nodiscard]] Dialogue& get_dialogue(const std::string&);

    // Set current dialogue data
    void set_current_dialogue(const dialogue_t&);

    // Set dialogue data
    void set_dialogue(const std::string&, const dialogue_t&);

    // Set dialogue data
    void set_dialogue(const std::string&, const nlohmann::json&);

    // Does dialogue with given id exist?
    bool contains_dialogue(const std::string&) const;

    // Remove current dialogue
    void remove_current_dialogue();

    // Remove dialogue
    void remove_dialogue(std::string);

private:
    std::unordered_map<std::string, Dialogue> dialog_data_;
    std::unordered_map<std::string, event_data_t> local_variables_;
    std::unordered_map<std::string, event_data_t> persistent_variables_;
    std::string current_dialog_id_ = "head";

    // Parse a string expression
    Number parse_expression(const std::string&) const;

    // Make sure dialogue exists
    void ensure_dialogue_existence(const std::string&) const;
};

#endif
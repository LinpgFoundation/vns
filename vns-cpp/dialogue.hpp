#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "dialogueNext.hpp"
#include "Event.hpp"

using dialogue_data_t = std::unordered_map<std::string, std::variant<std::string, std::vector<std::string>, std::unordered_map<std::string, dialogue_next_t>, std::vector<event_t>>>;

using dialogue_section_t = std::unordered_map<std::string, dialogue_data_t>;

using dialogue_content_t = std::unordered_map<std::string, dialogue_section_t>;

struct Dialogue
{
    Dialogue(const dialogue_data_t &, const std::string &);

    Dialogue() : Dialogue({}, "head")
    {
    }

    [[nodiscard]] bool has_next() const;

    void set_next(std::string, dialogue_next_t);

    void set_next(const std::unordered_map<std::string, dialogue_next_t> &);

    void remove_next();

    [[nodiscard]] dialogue_data_t to_map() const;

    [[nodiscard]] nlohmann::json to_json() const;

    std::string previous;
    DialogueNext next;
    std::string background_image;
    std::string background_music;
    std::vector<std::string> character_images;
    std::vector<std::string> contents;
    std::string narrator;
    std::vector<std::string> notes;
    std::vector<Event> events;
    std::string id;

    template<typename T> static T cast(const dialogue_data_t &data, const std::string &k, T default_v)
    {
        const auto it = data.find(k);
        if (it != data.end())
        {
            return std::get<T>(it->second);
        }
        return default_v;
    }
};

#endif

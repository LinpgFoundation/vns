#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "dialogueNext.hpp"
#include "event.hpp"

using dialogue_t = std::unordered_map<std::string, std::variant<
                                          std::string, std::vector<std::string>, dialogue_next_t, std::vector<event_t>>>
;

using dialogues_t = std::unordered_map<std::string, dialogue_t>;

class Dialogue
{
    template <typename T>
    static T cast(const dialogue_t& data, const std::string& k)
    {
        return data.contains(k) ? std::get<T>(data.at(k)) : T();
    }

    template <typename T>
    static T cast(const nlohmann::json& data, const std::string& k)
    {
        return data.contains(k) ? data.at(k).get<T>() : T();
    }

public:
    explicit Dialogue(const std::string&, const dialogue_t&);

    explicit Dialogue(const std::string&, const nlohmann::json&);

    explicit Dialogue(const std::string& content_id) : Dialogue(content_id, dialogue_t())
    {
    }

    Dialogue() : Dialogue("head")
    {
    }

    [[nodiscard]] bool has_previous() const;

    [[nodiscard]] bool has_next() const;

    void set_next(const std::string&, const dialogue_next_target_t&);

    void set_next(const dialogue_next_t&);

    void remove_next();

    [[nodiscard]] dialogue_t to_map() const;

    [[nodiscard]] nlohmann::json to_json() const;

    std::string previous;
    DialogueNext next;
    std::string background_image;
    std::string background_music;
    std::vector<std::string> character_images;
    std::vector<std::string> sound_effects;
    std::vector<std::string> contents;
    std::string narrator;
    std::vector<std::string> notes;
    std::vector<Event> events;
    std::string id;
};

#endif
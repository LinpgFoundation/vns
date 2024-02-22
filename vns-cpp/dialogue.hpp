#ifndef CONTENT_HPP
#define CONTENT_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include "dialogueNext.hpp"

using DialogueDataType = std::unordered_map<std::string, std::variant<std::string, std::vector<std::string>, std::unordered_map<std::string, DialogueNextValueType>>>;

using DialogueSectionDataType = std::unordered_map<std::string, DialogueDataType>;

using DialogueSectionsDataType = std::unordered_map<std::string, DialogueSectionDataType>;

struct Dialogue
{
    Dialogue(const DialogueDataType &, const std::string &);

    Dialogue() : Dialogue({}, "head")
    {
    }

    [[nodiscard]] bool has_next() const;

    void set_next(std::string, DialogueNextValueType);

    void set_next(const std::unordered_map<std::string, DialogueNextValueType> &);

    [[nodiscard]] DialogueDataType to_map() const;

    [[nodiscard]] nlohmann::json to_json() const;

    std::string previous;
    DialogueNext next;
    std::string background_image;
    std::string background_music;
    std::vector<std::string> character_images;
    std::vector<std::string> contents;
    std::string narrator;
    std::vector<std::string> notes;
    std::string id;

    template<typename T> static T cast(const DialogueDataType &data, const std::string &k, T default_v)
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

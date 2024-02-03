#ifndef CONTENT_H
#define CONTENT_H

#include <string>
#include <unordered_map>
#include <vector>
#include "contentNext.h"

using ContentDataType = std::unordered_map<std::string, std::variant<std::string, std::vector<std::string>, std::unordered_map<std::string, ContentNextValueType>>>;

using SectionDataType = std::unordered_map<std::string, ContentDataType>;

using DialogueDataType = std::unordered_map<std::string, SectionDataType>;

struct Content
{
    Content(const ContentDataType &, const std::string &);

    Content() : Content({}, "head")
    {
    }

    [[nodiscard]] bool has_next() const;

    [[nodiscard]] ContentDataType to_map() const;

    [[nodiscard]] nlohmann::json to_json() const;

    std::string previous;
    ContentNext next;
    std::string background_image;
    std::string background_music;
    std::vector<std::string> character_images;
    std::vector<std::string> contents;
    std::string narrator;
    std::vector<std::string> comments;
    std::string id;

    template<typename T> static T cast(const ContentDataType &data, const std::string &k, T default_v)
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

#include "content.h"

Content::Content(const ContentDataType &data, const std::string &content_id)
{
    id = content_id;
    background_image = cast<std::string>(data, "background_image", "");
    background_music = cast<std::string>(data, "background_music", "");
    character_images = cast<std::vector<std::string>>(data, "character_images", {});
    contents = cast<std::vector<std::string>>(data, "contents", {});
    narrator = cast<std::string>(data, "narrator", "");
    previous = cast<std::string>(data, "previous", "");
    next = ContentNext(cast<std::unordered_map<std::string, ContentNextValueType>>(data, "next", {{"type",   "default"},
                                                                                                  {"target", ""}}));
    comments = cast<std::vector<std::string>>(data, "comments", {});
}

bool Content::has_next() const
{
    return !next.is_null();
}

ContentDataType Content::to_map() const
{
    return {{"background_image", background_image},
            {"background_music", background_music},
            {"character_images", character_images},
            {"contents",         contents},
            {"previous",         previous},
            {"narrator",         narrator},
            {"next",             next.to_map()},
            {"comments",         comments}};
}

nlohmann::json Content::to_json() const
{
    nlohmann::json json_data;
    if (!background_image.empty())
        json_data["background_image"] = background_image;
    if (!background_music.empty())
        json_data["background_music"] = background_music;
    if (!character_images.empty())
        json_data["character_images"] = character_images;
    if (!contents.empty())
        json_data["contents"] = contents;
    if (!previous.empty())
        json_data["previous"] = previous;
    if (!narrator.empty())
        json_data["narrator"] = narrator;
    if (nlohmann::json next_in_json = next.to_json(); !next_in_json.empty())
        json_data["next"] = next_in_json;
    if (!comments.empty())
        json_data["comments"] = comments;
    return json_data;
}

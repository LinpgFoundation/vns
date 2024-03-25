#include "dialogue.hpp"

Dialogue::Dialogue(const DialogueDataType &data, const std::string &content_id)
{
    id = content_id;
    background_image = cast<std::string>(data, "background_image", std::string());
    background_music = cast<std::string>(data, "background_music", std::string());
    character_images = cast<std::vector<std::string>>(data, "character_images", {});
    contents = cast<std::vector<std::string>>(data, "contents", {});
    narrator = cast<std::string>(data, "narrator", std::string());
    previous = cast<std::string>(data, "previous", std::string());
    next = DialogueNext(cast<std::unordered_map<std::string, DialogueNextValueType>>(data, "next", {{"type",   "default"},
                                                                                                    {"target", std::string()}}));
    notes = cast<std::vector<std::string>>(data, "notes", {});
    for (const EventDataType &e: cast<std::vector<EventDataType>>(data, "events", {}))
    {
        events.emplace_back(e);
    }
}

bool Dialogue::has_next() const
{
    return !next.is_null();
}

void Dialogue::set_next(std::string type, DialogueNextValueType target)
{
    next = DialogueNext(std::move(type), std::move(target));
}

void Dialogue::set_next(const std::unordered_map<std::string, DialogueNextValueType> &data)
{
    next = data.empty() ? DialogueNext() : DialogueNext(data);
}

void Dialogue::remove_next()
{
    set_next({});
}

DialogueDataType Dialogue::to_map() const
{
    DialogueDataType map_data;
    if (!background_image.empty())
        map_data["background_image"] = background_image;
    if (!background_music.empty())
        map_data["background_music"] = background_music;
    if (!character_images.empty())
        map_data["character_images"] = character_images;
    if (!contents.empty())
        map_data["contents"] = contents;
    if (!previous.empty())
        map_data["previous"] = previous;
    if (!narrator.empty())
        map_data["narrator"] = narrator;
    if (has_next())
        map_data["next"] = next.to_map();
    if (!notes.empty())
        map_data["notes"] = notes;
    if (!events.empty())
    {
        std::vector<EventDataType> events_in_maps;
        for (const Event &e: events)
        {
            events_in_maps.push_back(e.to_map());
        }
        map_data["events"] = events_in_maps;
    }
    return map_data;
}

nlohmann::json Dialogue::to_json() const
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
    if (has_next())
        json_data["next"] = next.to_json();
    if (!notes.empty())
        json_data["notes"] = notes;
    if (!events.empty())
    {
        std::vector<nlohmann::json> events_in_maps;
        for (const Event &e: events)
        {
            events_in_maps.push_back(e.to_json());
        }
        json_data["events"] = events_in_maps;
    }
    return json_data;
}

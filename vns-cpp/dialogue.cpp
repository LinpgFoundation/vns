#include "dialogue.hpp"

Dialogue::Dialogue(const std::string &content_id, const dialogue_data_t &data)
{
    id = content_id;
    background_image = cast<std::string>(data, "background_image");
    background_music = cast<std::string>(data, "background_music");
    character_images = cast<std::vector<std::string>>(data, "character_images");
    contents = cast<std::vector<std::string>>(data, "contents");
    narrator = cast<std::string>(data, "narrator");
    previous = cast<std::string>(data, "previous");
    next = DialogueNext(cast<dialogue_next_t>(data, "next"));
    notes = cast<std::vector<std::string>>(data, "notes");
    for (const event_t &e: cast<std::vector<event_t>>(data, "events"))
    {
        events.emplace_back(e);
    }
}

Dialogue::Dialogue(const std::string &content_id, const nlohmann::json &data)
{
    id = content_id;
    background_image = cast<std::string>(data, "background_image");
    background_music = cast<std::string>(data, "background_music");
    character_images = cast<std::vector<std::string>>(data, "character_images");
    contents = cast<std::vector<std::string>>(data, "contents");
    narrator = cast<std::string>(data, "narrator");
    previous = cast<std::string>(data, "previous");
    if (data.contains("next"))
        next = DialogueNext(data.at("next"));
    notes = cast<std::vector<std::string>>(data, "notes");
    if (data.contains("events"))
    {
        for (const nlohmann::json &e: data.at("events"))
        {
            events.emplace_back(e);
        }
    }
}

bool Dialogue::has_next() const
{
    return !next.is_null();
}

void Dialogue::set_next(const std::string &type, const dialogue_next_target_t &target)
{
    next = DialogueNext(type, target);
}

void Dialogue::set_next(const dialogue_next_t &data)
{
    next = data.empty() ? DialogueNext() : DialogueNext(data);
}

void Dialogue::remove_next()
{
    set_next({});
}

dialogue_data_t Dialogue::to_map() const
{
    dialogue_data_t map_data;
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
        std::vector<event_t> events_in_maps;
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

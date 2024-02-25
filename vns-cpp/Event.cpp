#include "Event.hpp"

EventDataType Event::to_map() const
{
    return {{"type",   type},
            {"target", target},
            {"value",  value}};
}

nlohmann::json Event::to_json() const
{
    nlohmann::json json_data;
    json_data["type"] = type;
    json_data["target"] = target;
    json_data["value"] = value;
    return json_data;
}
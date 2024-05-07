#include "Event.hpp"

event_t Event::to_map() const
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
    std::visit([&json_data](auto &&arg) {
        // The lambda function takes an auto&& argument, which will deduce the type of the variant
        json_data["value"] = arg;
    }, value);
    return json_data;
}
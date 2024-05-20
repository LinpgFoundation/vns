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

Event Event::from_json(const nlohmann::json &data)
{
    event_data_t val;
    if (data.at("value").is_boolean())
    {
        val = data.at("value").get<bool>();
    } else if (data.at("value").is_number_integer())
    {
        val = data.at("value").get<int>();
    } else if (data.at("value").is_number_float())
    {
        val = data.at("value").get<float>();
    } else if (data.at("value").is_string())
    {
        val = data.at("value").get<std::string>();
    } else
    {
        throw std::runtime_error("Event data has unsupported type as value!");
    }

    return {data.at("type"), data.at("target"), val};
}

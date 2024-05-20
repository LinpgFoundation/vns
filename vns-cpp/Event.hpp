#ifndef VNS_EVENT_HPP
#define VNS_EVENT_HPP

#include <string>
#include <utility>
#include <unordered_map>
#include <variant>
#include <nlohmann/json.hpp>

using event_data_t = std::variant<bool, int, float, std::string>;
using event_t = std::unordered_map<std::string, event_data_t>;

struct Event
{

    Event(std::string type, std::string target, event_data_t value) : type(std::move(type)),
                                                                      target(std::move(target)),
                                                                      value(std::move(value))
    {
    }

    explicit Event(const event_t &data) : Event(std::get<std::string>(data.at("type")),
                                                std::get<std::string>(data.at("target")), data.at("value"))
    {
    }

    explicit Event(const nlohmann::json &data) : Event(data.at("type"),
                                                       data.at("target"), retrieve_value(data))
    {
    }

    [[nodiscard]] event_t to_map() const;

    [[nodiscard]] nlohmann::json to_json() const;

    [[nodiscard]] static event_data_t retrieve_value(const nlohmann::json &);

    const std::string type;
    const std::string target;
    const event_data_t value;
};


#endif //VNS_EVENT_HPP

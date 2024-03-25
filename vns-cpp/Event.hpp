#ifndef VNS_EVENT_HPP
#define VNS_EVENT_HPP

#include <string>
#include <utility>
#include <unordered_map>
#include <variant>
#include "libs/nlohmann/json.hpp"

using EventValueType = std::variant<bool, int, float, std::string>;
using EventDataType = std::unordered_map<std::string, EventValueType>;

struct Event
{

    Event(std::string type, std::string target, EventValueType value) : type(std::move(type)),
                                                                        target(std::move(target)),
                                                                        value(std::move(value))
    {
    }

    explicit Event(EventDataType data) : Event(std::get<std::string>(data["type"]),
                                               std::get<std::string>(data["target"]), data["value"])
    {
    }


    [[nodiscard]] EventDataType to_map() const;

    [[nodiscard]] nlohmann::json to_json() const;

    const std::string type;
    const std::string target;
    const EventValueType value;
};


#endif //VNS_EVENT_HPP

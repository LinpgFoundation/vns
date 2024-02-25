#ifndef VNS_EVENT_HPP
#define VNS_EVENT_HPP

#include <string>
#include <utility>
#include <unordered_map>
#include "libs/nlohmann/json.hpp"

using EventDataType = std::unordered_map<std::string, std::string>;

struct Event
{

    Event(std::string type, std::string target, std::string value) : type(std::move(type)), target(std::move(target)),
                                                                     value(std::move(value))
    {
    }

    explicit Event(EventDataType data) : Event(data["type"], data["target"], data["value"])
    {
    }


    [[nodiscard]] EventDataType to_map() const;

    [[nodiscard]] nlohmann::json to_json() const;

    const std::string type;
    const std::string target;
    const std::string value;
};


#endif //VNS_EVENT_HPP

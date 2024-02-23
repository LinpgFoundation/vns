#include "dialogueNext.hpp"

std::unordered_map<std::string, DialogueNextValueType> DialogueNext::to_map() const
{
    return {{"type",   type_},
            {"target", target_}};
}

nlohmann::json DialogueNext::to_json() const
{
    nlohmann::json json_data;
    if (!is_null())
    {
        json_data["type"] = type_;
        if (has_multi_targets())
        {
            json_data["target"] = get_targets();
        } else
        {
            json_data["target"] = get_target();
        }
    }
    return json_data;
}

bool DialogueNext::has_single_target() const
{
    return std::holds_alternative<std::string>(target_);
}

bool DialogueNext::has_multi_targets() const
{
    return !has_single_target();
}

std::string DialogueNext::get_type() const
{
    return type_;
}

bool DialogueNext::has_type(const std::string &type) const
{
    return type_ == type;
}

std::string DialogueNext::get_target() const
{
    return std::get<std::string>(target_);
}

MultiTargetsType DialogueNext::get_targets() const
{
    return std::get<MultiTargetsType>(target_);
}

bool DialogueNext::is_null() const
{
    return has_single_target() ? get_target().empty() : get_targets().empty();
}

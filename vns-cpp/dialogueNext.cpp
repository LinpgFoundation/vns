#include "dialogueNext.hpp"

bool DialogueNext::has_single_target() const
{
    return std::holds_alternative<std::string>(target_);
}

bool DialogueNext::has_multi_targets() const
{
    return !has_single_target();
}

bool DialogueNext::contains_target(const std::string &theTarget) const
{
    return has_single_target() ? get_target() == theTarget : std::ranges::any_of(get_targets(),
                                                                                 [&theTarget](const auto &pair) {
                                                                                     return pair.at("id") == theTarget;
                                                                                 });
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

multi_targets_t DialogueNext::get_targets() const
{
    return std::get<multi_targets_t>(target_);
}

bool DialogueNext::is_null() const
{
    return has_single_target() ? get_target().empty() : get_targets().empty();
}

dialogue_next_t DialogueNext::to_map() const
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

dialogue_next_target_t DialogueNext::retrieve_target(const nlohmann::json &data)
{
    dialogue_next_target_t target_;
    if (!data.contains("target"))
    {
        target_ = std::string();
    } else if (data.at("target").is_string())
    {
        target_ = data.at("target").get<std::string>();
    } else
    {
        target_ = data.at("target").get<std::vector<std::unordered_map<std::string, std::string>>>();
    }
    return target_;
}

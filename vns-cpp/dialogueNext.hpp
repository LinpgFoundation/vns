#ifndef CONTENT_NEXT_HPP
#define CONTENT_NEXT_HPP

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>
#include <vector>
#include "libs/nlohmann/json.hpp"

using MultiTargetsType = std::vector<std::unordered_map<std::string, std::string>>;

using DialogueNextValueType = std::variant<std::string, MultiTargetsType>;

class DialogueNext
{
public:
    DialogueNext(std::string type, DialogueNextValueType target) : type_(std::move(type)), target_(std::move(target))
    {
    }

    explicit DialogueNext(const std::unordered_map<std::string, DialogueNextValueType> &data) : DialogueNext(
            data.contains("type") ? std::get<std::string>(data.at("type")) : "default",
            data.contains("target") ? data.at("target") : "")
    {
    }

    DialogueNext() : DialogueNext("default", "")
    {
    }

    [[nodiscard]] std::string get_type() const;

    [[nodiscard]] bool has_type(const std::string &) const;

    [[nodiscard]] std::string get_target() const;

    [[nodiscard]] MultiTargetsType get_targets() const;

    [[nodiscard]] bool has_single_target() const;

    [[nodiscard]] bool has_multi_targets() const;

    [[nodiscard]] bool is_null() const;

    [[nodiscard]] std::unordered_map<std::string, DialogueNextValueType> to_map() const;

    [[nodiscard]] nlohmann::json to_json() const;

private:
    std::string type_;
    DialogueNextValueType target_;
};

const DialogueNext kNullDialogueNext;
#endif

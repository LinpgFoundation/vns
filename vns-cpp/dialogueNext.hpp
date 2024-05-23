#ifndef CONTENT_NEXT_HPP
#define CONTENT_NEXT_HPP

#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>
#include <vector>
#include <nlohmann/json.hpp>

using multi_targets_t = std::vector<std::unordered_map<std::string, std::string>>;

using dialogue_next_target_t = std::variant<std::string, multi_targets_t>;

using dialogue_next_t = std::unordered_map<std::string, dialogue_next_target_t>;

class DialogueNext
{
public:
    DialogueNext(std::string type, dialogue_next_target_t target) : type_(std::move(type)), target_(std::move(target))
    {
    }

    explicit DialogueNext(const dialogue_next_t &data) : DialogueNext(
            data.contains("type") ? std::get<std::string>(data.at("type")) : "default",
            data.contains("target") ? data.at("target") : std::string())
    {
    }

    explicit DialogueNext(const nlohmann::json &data) : DialogueNext(
            data.contains("type") ? data.at("type") : "default", retrieve_target(data))
    {
    }

    DialogueNext() : DialogueNext("default", std::string())
    {
    }

    [[nodiscard]] std::string get_type() const;

    [[nodiscard]] bool has_type(const std::string &) const;

    [[nodiscard]] std::string get_target() const;

    [[nodiscard]] std::vector<std::unordered_map<std::string, std::string>> get_targets() const;

    [[nodiscard]] bool has_single_target() const;

    [[nodiscard]] bool has_multi_targets() const;

    [[nodiscard]] bool contains_target(const std::string &) const;

    [[nodiscard]] bool is_null() const;

    [[nodiscard]] dialogue_next_t to_map() const;

    [[nodiscard]] nlohmann::json to_json() const;

private:
    std::string type_;
    dialogue_next_target_t target_;

    [[nodiscard]] static dialogue_next_target_t retrieve_target(const nlohmann::json &);
};

#endif

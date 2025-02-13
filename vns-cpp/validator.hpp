#ifndef VALIDATOR_HPP
#define VALIDATOR_HPP

#include <nlohmann/json-schema.hpp>

class Validator
{
    static const nlohmann::json_schema::json_validator VALIDATOR;

public:
    // validate whether given json object is valid vsn format dialogues
    [[nodiscard]] static bool validate(const nlohmann::json &);

    // validate whether given json file has valid vsn format dialogues
    [[nodiscard]] static bool validate(const std::filesystem::path &);

    // ensure given json object is valid vsn format dialogues, throw error if not
    static void ensure(const nlohmann::json &);

    // ensure given json file has valid vsn format dialogues, throw error if not
    static void ensure(const std::filesystem::path &);
};

#endif

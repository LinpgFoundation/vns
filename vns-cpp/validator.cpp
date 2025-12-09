#include "validator.hpp"
#include "functions.hpp"
#include "schema.hpp"

// Lazy initialization of validator to avoid static initialization order fiasco
const nlohmann::json_schema::json_validator& Validator::get_validator()
{
    static const nlohmann::json_schema::json_validator validator(VNS_SCHEMA_JSON);
    return validator;
}

// validate whether given json object is valid vsn format dialogues
bool Validator::validate(const nlohmann::json &jsonObj)
{
    try
    {
        get_validator().validate(jsonObj); // validate the document - uses the default throwing error-handler
        return true;
    } catch ([[maybe_unused]] const std::exception &e) {
        return false;
    }
}

// validate whether given json file has valid vsn format dialogues
bool Validator::validate(const std::filesystem::path &jsonPath)
{
    return validate(load_json(jsonPath));
}

// ensure given json object is valid vsn format dialogues, throw error if not
void Validator::ensure(const nlohmann::json &jsonObj)
{
    if (!validate(jsonObj))
    {
        throw std::runtime_error("Given json object is not valid vsn format dialogues!");
    }
}

// ensure given json file has valid vsn format dialogues, throw error if not
void Validator::ensure(const std::filesystem::path &jsonPath)
{
    if (!validate(jsonPath))
    {
        throw std::runtime_error("Given json file does not have valid format: " + jsonPath.string());
    }
}
#include "validator.hpp"
#include "functions.hpp"

// validate whether given json object is valid vsn format dialogues
bool Validator::validate(const nlohmann::json &jsonObj) const
{
    try
    {
        VALIDATOR.validate(jsonObj); // validate the document - uses the default throwing error-handler
        return true;
    } catch (const std::exception &e)
    {
        return false;
    }
}

// validate whether given json file has valid vsn format dialogues
bool Validator::validate(const std::filesystem::path &jsonPath) const
{
    return validate(load_json(jsonPath));
}

// ensure given json object is valid vsn format dialogues, throw error if not
void Validator::ensure(const nlohmann::json &jsonObj) const
{
    if (!validate(jsonObj))
    {
        throw std::runtime_error("Given json object is not valid vsn format dialogues!");
    }
}

// ensure given json file has valid vsn format dialogues, throw error if not
void Validator::ensure(const std::filesystem::path &jsonPath) const
{
    if (!validate(jsonPath))
    {
        throw std::runtime_error("Given json file does not have valid format: " + jsonPath.string());
    }
}
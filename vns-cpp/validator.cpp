#include "validator.hpp"
#include "functions.hpp"

// validate whether given json object is valid vsn format dialogue
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

// validate whether a json file has valid vsn format dialogue
bool Validator::validate(const std::filesystem::path &jsonPath) const
{
    return validate(load_json(jsonPath));
}
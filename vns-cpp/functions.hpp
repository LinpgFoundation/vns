#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

bool ichar_equals(char, char);

bool iequals(std::string_view, std::string_view);

// trim from start
std::string ltrim(std::string);

// trim from end
std::string rtrim(std::string);

// trim from both ends
std::string trim(const std::string &);

// convert string to lower case
std::string to_lower(const std::string &);

// split by given character
std::vector<std::string> split(const std::string &, char);

// split by space
std::vector<std::string> split(const std::string &);

// Remove all whitespaces
std::string remove_whitespace(const std::string &);

// Load a json file
nlohmann::json load_json(const std::filesystem::path &);

// Save a json file
void save_json(const std::filesystem::path &, const nlohmann::json &);

#endif

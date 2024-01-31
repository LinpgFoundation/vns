#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>
#include <vector>

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

#endif

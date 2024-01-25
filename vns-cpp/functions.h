#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <string>

bool ichar_equals(char, char);

bool iequals(std::string_view, std::string_view);

// trim from start (in place)
std::string ltrim(std::string);

// trim from end (in place)
std::string rtrim(std::string);

// trim from both ends (in place)
std::string trim(std::string);

// convert string to lower case
std::string toLowerCase(const std::string&);

#endif

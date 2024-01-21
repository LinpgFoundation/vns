#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include <string>

bool ichar_equals(char, char);

bool iequals(std::string_view lhs, std::string_view rhs);

// trim from start (in place)
void ltrim(std::string&);

// trim from end (in place)
void rtrim(std::string&);

// trim from both ends (in place)
std::string trim(std::string);

#endif

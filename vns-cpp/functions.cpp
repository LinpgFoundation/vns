#include "functions.h"
#include <algorithm>

bool ichar_equals(char a, char b)
{
	return std::tolower(static_cast<unsigned char>(a)) ==
		std::tolower(static_cast<unsigned char>(b));
}

bool iequals(std::string_view lhs, std::string_view rhs)
{
	return std::ranges::equal(lhs, rhs, ichar_equals);
}

// trim from start (in place)
void ltrim(std::string& s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch)
	{
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
void rtrim(std::string& s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch)
	{
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
std::string trim(std::string s)
{
	ltrim(s);
	rtrim(s);
	return s;
}

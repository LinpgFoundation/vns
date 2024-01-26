#include "functions.h"
#include <algorithm>
#include <sstream>

bool ichar_equals(const char a, const char b)
{
	return std::tolower(static_cast<unsigned char>(a)) ==
		std::tolower(static_cast<unsigned char>(b));
}

bool iequals(std::string_view lhs, std::string_view rhs)
{
	return std::ranges::equal(lhs, rhs, ichar_equals);
}

// trim from start
std::string ltrim(std::string s)
{
	s.erase(s.begin(), std::ranges::find_if(s.begin(), s.end(), [](const unsigned char ch)
	{
		return !std::isspace(ch);
	}));
	return s;
}

// trim from end
std::string rtrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](const unsigned char ch)
	{
		return !std::isspace(ch);
	}).base(), s.end());
	return s;
}

// trim from both ends
std::string trim(const std::string& s)
{
	return rtrim(ltrim(s));
}

// convert string to lower case
std::string to_lower(const std::string& input)
{
	std::string result = input;

	for (char& c : result)
	{
		c = std::tolower(c);
	}

	return result;
}

// split by given character
std::vector<std::string> split(const std::string& str, const char c)
{
	std::string s;
	std::stringstream ss(str);
	std::vector<std::string> result;

	while (getline(ss, s, c))
	{
		if (!s.empty())
		{
			result.push_back(s);
		}
	}

	return result;
}

// split by space
std::vector<std::string> split(const std::string& str)
{
	return split(str, ' ');
}

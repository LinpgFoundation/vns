#include "functions.hpp"
#include <algorithm>
#include <sstream>
#include <fstream>

bool ichar_equals(const char a, const char b)
{
    return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b));
}

bool iequals(std::string_view lhs, std::string_view rhs)
{
    return std::ranges::equal(lhs, rhs, ichar_equals);
}

// trim from start
std::string ltrim(std::string s)
{
    s.erase(s.begin(), std::ranges::find_if(s.begin(), s.end(), [](const unsigned char ch) {
        return !std::isspace(ch);
    }));
    return s;
}

// trim from end
std::string rtrim(std::string s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](const unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
    return s;
}

// trim from both ends
std::string trim(const std::string &s)
{
    return rtrim(ltrim(s));
}

// convert string to lower case
std::string to_lower(const std::string &input)
{
    std::string result = input;

    for (char &c: result)
    {
        c = static_cast<char>(std::tolower(c));
    }

    return result;
}

// split by given character
std::vector<std::string> split(const std::string &str, const char c)
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
std::vector<std::string> split(const std::string &str)
{
    return split(str, ' ');
}

// Remove all whitespaces
std::string remove_whitespace(const std::string &input)
{
    std::string result = input; // Make a copy of the input string
    result.erase(std::remove_if(result.begin(), result.end(), [](unsigned char x) {
        return std::isspace(x);
    }), result.end());
    return result;
}

// Load a json file
nlohmann::json load_json(const std::filesystem::path &jsonPath)
{
    std::ifstream json_file(jsonPath);
    nlohmann::json j;
    if (json_file.is_open())
    {
        try
        {
            json_file >> j;  // Deserializing JSON file to the json object
            return j;
        } catch (nlohmann::json::parse_error &e)
        {
            std::stringstream errMsg;
            errMsg << "Cannot read json file at path: " << jsonPath << "\nReason: " << e.what();
            throw std::runtime_error(errMsg.str());
        }
    } else
    {
        std::stringstream errMsg;
        errMsg << "Cannot open json file at path: " << jsonPath;
        throw std::runtime_error(errMsg.str());
    }
}

// Save a json file
void save_json(const std::filesystem::path &jsonPath, const nlohmann::json &jsonData)
{
    std::ofstream outputFile(jsonPath);
    // Check if the file is open
    if (outputFile.is_open())
    {
        // Save the JSON object to the file
        outputFile << std::setw(4) << jsonData << "\n";
        // Close the file stream
        outputFile.close();
    } else
    {
        std::stringstream errMsg;
        errMsg << "Cannot open json file at path: " << jsonPath;
        throw std::runtime_error(errMsg.str());
    }
}
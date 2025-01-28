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
        result.push_back(s);
    }

    return result;
}

// split by space
std::vector<std::string> split(const std::string &str)
{
    return split(str, ' ');
}

// join strings with delimiter
std::string join(const std::vector<std::string> &strings_vec, const std::string &delimiter)
{
    std::stringstream ss;
    if (!strings_vec.empty())
    {
        // Use a lambda function with for_each to iterate and append
        std::for_each(strings_vec.begin(), strings_vec.end() - 1, [&](const std::string &s) {
            ss << s << delimiter;  // Append string and delimiter
        });
        ss << strings_vec.back().c_str();  // Append the last element without delimiter
    }
    return ss.str();
}

// Remove all whitespaces
std::string remove_whitespace(const std::string &input)
{
    std::string result = input; // Make a copy of the input string
    result.erase(std::ranges::remove_if(result, [](const unsigned char x) {
        return std::isspace(x);
    }).begin(), result.end());
    return result;
}

// Load a json file
nlohmann::json load_json(const std::filesystem::path &jsonPath)
{
    if (std::ifstream json_file(jsonPath); json_file.is_open()) {
        try {
            nlohmann::json j;
            json_file >> j;  // Deserializing JSON file to the json object
            return j;
        } catch (nlohmann::json::parse_error &e)
        {
            std::stringstream errMsg;
            errMsg << "Cannot read json file at path: " << jsonPath << "\nReason: " << e.what();
            throw std::runtime_error(errMsg.str());
        }
    }
    // throw cannot open file error
    std::stringstream errMsg;
    errMsg << "Cannot open json file at path: " << jsonPath;
    throw std::runtime_error(errMsg.str());
}

// Save a json file
void save_json(const std::filesystem::path &jsonPath, const nlohmann::json &jsonData)
{
    // Check if the file is open
    if (std::ofstream outputFile(jsonPath); outputFile.is_open()) {
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

// Load a plain text file and return contents as lines
std::vector<std::string> load_file_as_lines(const std::filesystem::path &filePath)
{
    std::vector<std::string> lines;
    load_file_as_lines(filePath, lines);
    return lines;
}

// Load a plain text file and push the contents to out
void load_file_as_lines(const std::filesystem::path &filePath, std::vector<std::string> &out)
{
    if (std::ifstream f(filePath); f.is_open())
    {
        std::string line;
        while (std::getline(f, line))
        {
            out.push_back(line);
        }
        f.close();
    } else
    {
        std::stringstream errMsg;
        errMsg << "File " << filePath << " is occupied!";
        throw std::runtime_error(errMsg.str());
    }
}
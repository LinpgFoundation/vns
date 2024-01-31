#ifndef COMPILER_H
#define COMPILER_H

#include <filesystem>
#include "libs/nlohmann/json.hpp"

class Compiler
{
public:
    static std::unordered_map<std::string, int> get_compiler_info();

    static std::unordered_map<std::string, std::any> load(const std::filesystem::path &);

    static void compile(const std::filesystem::path &, const std::filesystem::path & = "");

    static std::string load_as_string(const std::filesystem::path &);

private:
    static nlohmann::json load_as_json(const std::filesystem::path &);

    static void save(const nlohmann::json &, const std::filesystem::path &);
};

#endif

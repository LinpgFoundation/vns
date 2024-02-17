#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <filesystem>
#include <unordered_map>
#include "libs/nlohmann/json.hpp"
#include "scriptProcessor.hpp"

using DialogueFileDataType = std::unordered_map<std::string, std::variant<DialogueSectionsDataType, std::unordered_map<std::string, int>, std::string>>;

class Compiler
{
public:
    static std::unordered_map<std::string, int> get_compiler_info();

    static DialogueFileDataType load(const std::filesystem::path &);

    static void compile(const std::filesystem::path &);

    static void compile(const std::filesystem::path &, const std::filesystem::path &);

    static std::string load_as_string(const std::filesystem::path &);

private:
    static nlohmann::json load_as_json(const std::filesystem::path &);

    static void save(const nlohmann::json &, const std::filesystem::path &);
};

#endif

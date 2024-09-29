#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <thread>
#include <filesystem>
#include <unordered_map>
#include <nlohmann/json.hpp>
#include "scriptProcessor.hpp"

class Compiler
{
public:
    static std::unordered_map<std::string, size_t> get_compiler_info();

    static std::unordered_map<std::string, std::variant<dialogue_content_t, std::unordered_map<std::string, size_t>, std::string>>
    load(const std::filesystem::path &);

    static void compile(const std::filesystem::path &);

    static void compile(const std::filesystem::path &, const std::filesystem::path &);

    static void parallel_compile(const std::filesystem::path &);

    static void parallel_compile(const std::filesystem::path &, const std::filesystem::path &);

    static std::string load_as_string(const std::filesystem::path &);

    static std::string load_as_string(const std::string &);

private:
    static nlohmann::json load_as_json(const std::filesystem::path &);

    static nlohmann::json load_as_json(const std::string &);

    static nlohmann::json output(const ScriptProcessor &);

    static void save(const nlohmann::json &, const std::filesystem::path &);

    static void compile_script(const std::filesystem::path &, const std::filesystem::path &);

    static void add_tasks(const std::filesystem::path &, const std::filesystem::path &, std::vector<std::thread> &);
};

#endif

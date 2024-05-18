#include <ctime>
#include "version.hpp"
#include "compiler.hpp"
#include <iostream>
#include "functions.hpp"

// get the info of compiler
std::unordered_map<std::string, size_t> Compiler::get_compiler_info()
{
    return {{"version",    VERSION},
            {"reversion",  REVISION},
            {"compiledAt", static_cast<size_t>(std::time(nullptr))}};
}

// load data from file directly
std::unordered_map<std::string, std::variant<dialogue_content_t, std::unordered_map<std::string, size_t>, std::string>>
Compiler::load(const std::filesystem::path &path)
{
    ScriptProcessor processor;
    processor.process(path);
    return {{"dialogs",  processor.get_output().to_map()},
            {"compiler", get_compiler_info()},
            {"id",       processor.get_id()},
            {"language", processor.get_language()}};
}

// load data in the form of json string from file directly
std::string Compiler::load_as_string(const std::filesystem::path &path)
{
    return nlohmann::to_string(load_as_json(path));
}

// load data in the form of json data from file directly
nlohmann::json Compiler::load_as_json(const std::filesystem::path &path)
{
    ScriptProcessor processor;
    processor.process(path);
    nlohmann::json json_data;
    json_data["dialogs"] = processor.get_output().to_json();
    json_data["compiler"] = get_compiler_info();
    json_data["id"] = processor.get_id();
    json_data["language"] = processor.get_language();
    return json_data;
}

// compile the file
void Compiler::compile(const std::filesystem::path &path)
{
    compile(path, path.root_directory());
}

void Compiler::compile(const std::filesystem::path &path, const std::filesystem::path &out_dir)
{
    if (!is_directory(path))
    {
        save(load_as_json(path), out_dir);
    } else
    {
        for (const auto &entry: std::filesystem::directory_iterator(path))
        {
            if (entry.path().extension() == ScriptProcessor::SCRIPTS_FILE_EXTENSION)
            {
                compile(entry.path());
            }
        }
    }
}

// save the json data into json
void Compiler::save(const nlohmann::json &json_data, const std::filesystem::path &dir_path)
{
    std::stringstream file_name;
    std::string id = json_data["id"];
    std::string lang = json_data["language"];
    file_name << "chapter" << id << "_dialogs_" << lang << ".json";
    save_json(dir_path / file_name.str(), json_data);
}

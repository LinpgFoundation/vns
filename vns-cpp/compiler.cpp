#include <ctime>
#include "version.hpp"
#include "compiler.hpp"
#include "functions.hpp"

// get the info of compiler
std::unordered_map<std::string, size_t> Compiler::get_compiler_info()
{
    return {
        {"version", VERSION},
        {"reversion", REVISION},
        {"patch", PATCH},
        {"compiledAt", static_cast<size_t>(std::time(nullptr))}
    };
}

// load data from the file directly
std::unordered_map<std::string, std::variant<dialogue_content_t, std::unordered_map<std::string, size_t>, std::string>>
Compiler::load(const std::filesystem::path& path)
{
    ScriptProcessor processor;
    processor.process(path);
    return {
        {"dialogues", processor.get_output().to_map()},
        {"compiler", get_compiler_info()},
        {"id", processor.get_id()},
        {"language", processor.get_language()}
    };
}

// load data in the form of JSON string from the file directly
std::string Compiler::load_as_string(const std::filesystem::path& path)
{
    return nlohmann::to_string(load_as_json(path));
}

// load data in the form of JSON string from raw vns string data directly
std::string Compiler::load_as_string(const std::string& raw_data)
{
    return nlohmann::to_string(load_as_json(raw_data));
}

// load data in the form of JSON data from the file directly
nlohmann::json Compiler::load_as_json(const std::filesystem::path& path)
{
    ScriptProcessor processor;
    processor.process(path);
    return output(processor);
}

// load data in the form of JSON data from raw vns string data directly
nlohmann::json Compiler::load_as_json(const std::string& raw_data)
{
    ScriptProcessor processor;
    processor.process(raw_data);
    return output(processor);
}

nlohmann::json Compiler::output(const ScriptProcessor& processor)
{
    nlohmann::json json_data;
    json_data["dialogues"] = processor.get_output().to_json();
    json_data["compiler"] = get_compiler_info();
    json_data["id"] = processor.get_id();
    json_data["language"] = processor.get_language();
    return json_data;
}

// compile the file/files
void Compiler::compile(const std::filesystem::path& path)
{
    compile(path, is_directory(path) ? path : path.parent_path());
}

void Compiler::compile(const std::filesystem::path& path, const std::filesystem::path& out_dir)
{
    if (is_directory(path))
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (entry.path().extension() == ScriptProcessor::SCRIPTS_FILE_EXTENSION)
            {
                compile_script(entry.path(), out_dir);
            }
            else if (entry.is_directory())
            {
                compile(entry.path(), out_dir / entry.path().filename());
            }
        }
    }
    else if (path.extension() == ScriptProcessor::SCRIPTS_FILE_EXTENSION)
    {
        compile_script(path, out_dir);
    }
}

// compile a script file and save it to the given output dir
void Compiler::compile_script(const std::filesystem::path& path, const std::filesystem::path& out_dir)
{
    save(load_as_json(path), out_dir);
}

// compile the file/files using multithreading
void Compiler::parallel_compile(const std::filesystem::path& path)
{
    parallel_compile(path, is_directory(path) ? path : path.parent_path());
}

void Compiler::parallel_compile(const std::filesystem::path& path, const std::filesystem::path& out_dir)
{
    std::vector<std::thread> tasks;
    add_tasks(path, out_dir, tasks);
    for (std::thread& task : tasks)
        task.join();
}

// create thread(s) that start compiling script(s)
void Compiler::add_tasks(
    const std::filesystem::path& path, const std::filesystem::path& out_dir, std::vector<std::thread>& tasks)
{
    if (is_directory(path))
    {
        for (const auto& entry : std::filesystem::directory_iterator(path))
        {
            if (const std::filesystem::path& entryPath = entry.path();
                entryPath.extension() == ScriptProcessor::SCRIPTS_FILE_EXTENSION)
            {
                tasks.emplace_back([entryPath, out_dir]
                {
                    compile_script(entryPath, out_dir);
                });
            }
            else if (entry.is_directory())
            {
                add_tasks(entryPath, out_dir / entryPath.filename(), tasks);
            }
        }
    }
    else if (path.extension() == ScriptProcessor::SCRIPTS_FILE_EXTENSION)
    {
        tasks.emplace_back([path, out_dir]
        {
            compile_script(path, out_dir);
        });
    }
}

// save the JSON data
void Compiler::save(const nlohmann::json& json_data, const std::filesystem::path& dir_path)
{
    // check the output dir if it does not exist
    if (!exists(dir_path))
        create_directories(dir_path);
    // get the id of the dialogue
    const std::string id = json_data.at("id");
    // set the file name for output JSON
    const std::string file_name = id + ".json";
    // save data
    save_json(dir_path / file_name, json_data);
}
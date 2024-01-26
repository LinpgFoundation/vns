#include <ctime>
#include "version.h"
#include "compiler.h"
#include <fstream>
#include <iostream>
#include "libs/nlohmann/json.hpp"

// get the info of compiler
std::unordered_map<std::string, int> Compiler::get_compiler_info()
{
	return {
		{"version", VERSION},
		{"reversion", REVISION},
		{"patch", PATCH},
		{"compiledAt", static_cast<int>(std::time(nullptr))}
	};
}

// load data from file directly
std::unordered_map<std::string, std::any> Compiler::load(const std::filesystem::path& path)
{
	Processor _processor;
	_processor.process(path);
	return {
		{"dialogs", _processor.get_output()},
		{"compiler", get_compiler_info()},
		{"id", _processor.get_id()},
		{"language", _processor.get_language()}
	};
}

// load data in the form of json string from file directly
std::string Compiler::load_as_string(const std::filesystem::path& path)
{
	return nlohmann::to_string(load_as_json(path));
}

// load data in the form of json data from file directly
nlohmann::json Compiler::load_as_json(const std::filesystem::path& path)
{
	Processor _processor;
	_processor.process(path);
	nlohmann::json json_data;
	json_data["dialogs"] = _processor.get_output_as_json();
	json_data["compiler"] = get_compiler_info();
	json_data["id"] = _processor.get_id();
	json_data["language"] = _processor.get_language();
	return json_data;
}

// compile the file
void Compiler::compile(const std::filesystem::path& path, const std::filesystem::path& out_dir)
{
	if (!is_directory(path))
	{
		save(load_as_json(path), out_dir.empty() ? path.root_directory() : out_dir);
	}
	else
	{
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if (entry.path().extension() == Processor::SCRIPTS_FILE_EXTENSION)
			{
				compile(entry.path());
			}
		}
	}
}

// save the json data into json
void Compiler::save(const nlohmann::json& json_data, const std::filesystem::path& dir_path)
{
	std::stringstream file_name;
	std::string id = json_data["id"];
	std::string lang = json_data["language"];
	file_name << "chapter" << id << "_dialogs_" << lang << ".json";
	const std::filesystem::path file_path = dir_path / file_name.str();
	std::ofstream outputFile(file_path);
	// Check if the file is open
	if (outputFile.is_open())
	{
		// Save the JSON object to the file
		outputFile << std::setw(4) << json_data << "\n";
		std::cout << "JSON data saved to file: " << file_path << "\n";
		// Close the file stream
		outputFile.close();
	}
	else
	{
		std::cerr << "Error opening file: " << file_path << "\n";
	}
}

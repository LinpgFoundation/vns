// vns-cpp.cpp : Defines the entry point for the application.
//

#include "vns.h"
#include <unordered_map>
#include <unordered_set>
#include "tests.h"
#include "compiler.h"
#include "naming.h"

int main(int argc, char** argv)
{
	const std::unordered_set<std::string> arguments_with_input = {"-i", "-o", "-n"};
	const std::unordered_set<std::string> arguments_without_input = {"-h", "-s", "-t"};
	std::unordered_map<std::string, std::string> arguments_map;
	for (int i = 1; i < argc; i++)
	{
		std::string current_arg = argv[i];
		if (arguments_with_input.contains(current_arg))
		{
			arguments_map[current_arg] = argv[++i];
		}
		else if (arguments_without_input.contains(current_arg))
		{
			arguments_map[current_arg] = "";
		}
	}
	// update naming database
	if (arguments_map.contains("-n"))
	{
		Naming::update_database(nlohmann::json::parse(arguments_map["-n"]));
	}
	// run tests
	if (arguments_map.contains("-t"))
	{
		TestAll();
	}
	// compile file and save as json
	if (arguments_map.contains("-i"))
	{
		// save as json
		if (arguments_map.contains("-s"))
		{
			std::cout << Compiler::load_as_string(arguments_map["-i"]) << "\n";
		}
		// print json string
		else
		{
			Compiler::compile(arguments_map["-i"], arguments_map.contains("-o") ? arguments_map["-o"] : "");
		}
	}
	return 0;
}

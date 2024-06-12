// vns-cpp.cpp : Defines the entry point for the application.
//

#include "vns.hpp"
#include <unordered_map>
#include <unordered_set>
#include "tests.hpp"
#include "compiler.hpp"
#include "naming.hpp"

int main(const int argc, char **argv)
{
    const std::unordered_set<std::string> arguments_with_input = {"-i", "-o", "-n"};
    const std::unordered_set<std::string> arguments_without_input = {"-h", "-s", "-t"};
    std::unordered_map<std::string, std::string> arguments_map;
    for (size_t i = 1; i < argc; ++i)
    {
        if (arguments_with_input.contains(argv[i]))
        {
            arguments_map[argv[i]] = argv[++i];
        } else if (arguments_without_input.contains(argv[i]))
        {
            arguments_map[argv[i]].clear();
        }
    }
    // update naming database
    if (arguments_map.contains("-n"))
        Naming::update_database(arguments_map.at("-n"));
    // run tests
    if (arguments_map.contains("-t"))
        TestAll();
    // compile file and save as json
    if (arguments_map.contains("-i"))
    {
        // save as json
        if (arguments_map.contains("-s"))
        {
            std::cout << Compiler::load_as_string(arguments_map.at("-i")) << "\n";
        }
            // print json string
        else
        {
            Compiler::compile(arguments_map.at("-i"),
                              arguments_map.contains("-o") ? arguments_map.at("-o") : std::string());
        }
    }
    return 0;
}

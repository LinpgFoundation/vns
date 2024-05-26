#include "httplib.h"
#include "extern/vns-cpp/compiler.hpp"
#include "extern/vns-cpp/schema.hpp"

// HTTP
int main(const int argc, char **argv)
{
    // process arguments
    const std::unordered_set<std::string> arguments_with_input = {"-p", "-a"};
    const std::unordered_set<std::string> arguments_without_input = {"-d"};
    std::unordered_map<std::string, std::string> arguments_map;
    for (size_t i = 1; i < argc; ++i)
    {
        if (std::string current_arg = argv[i]; arguments_with_input.contains(current_arg))
        {
            arguments_map[current_arg] = argv[++i];
        } else if (arguments_without_input.contains(current_arg))
        {
            arguments_map[current_arg].clear();
        } else
        {
            std::cout << "Unknown argument " << current_arg << ", ignored." << std::endl;
        }
    }

    // address
    const std::string ADDRESS = arguments_map.contains("-a") ? arguments_map.at("-a") : "0.0.0.0";

    // port
    const int PORT = arguments_map.contains("-p") ? stoi(arguments_map.at("-p")) : 8181;

    // if show debug info
    const bool DEBUG = arguments_map.contains("-d");

    // Create a server object
    httplib::Server svr;

    // Define a route for handling compile request
    svr.Post("/compile", [&DEBUG](const httplib::Request &req, httplib::Response &res) {
        // Output the request body
        if (DEBUG)
            std::cout << "Received POST request with body: " << req.body << std::endl;

        // parse body to json
        nlohmann::json req_j = nlohmann::json::parse(req.body);

        // retire raw vns data from json
        const std::string data = req_j["data"];

        // compile raw vns data
        const std::string compiled = Compiler::load_as_string(data);

        if (DEBUG)
            std::cout << "result: " << compiled << std::endl;

        // Respond with compile json data
        res.set_content(compiled, "application/json");
    });

    // Define a route for handling get schema request
    svr.Get("/get/schema", [](const httplib::Request &, httplib::Response &res) {
        res.set_content(VNS_SCHEMA, "application/json");
    });

    // Start the server on given address (at localhost:8181 by default)
    std::cout << "Starting server at http://" << ADDRESS << ':' << PORT << std::endl;

    // listen the port
    svr.listen(ADDRESS, PORT);

    return 0;
}
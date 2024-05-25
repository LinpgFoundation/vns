cmake_minimum_required(VERSION 3.28)

project(vns_server)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add vns-cpp
add_subdirectory(extern/vns-cpp)

# Add cpp-httplib
add_subdirectory(extern/cpp-httplib)

# Add the executable
add_executable(vns_server server.cpp)

# Link against libraries
target_link_libraries(vns_server PRIVATE nlohmann_json::nlohmann_json nlohmann_json_schema_validator vns_lib httplib)

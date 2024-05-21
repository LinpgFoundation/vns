# set cmake ecn
cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(vns_python_wrapper)

# find python
find_package(Python COMPONENTS Interpreter Development)

# add pybind11
add_subdirectory(extern/pybind11)

# add vns-cpp
add_subdirectory(extern/vns-cpp)

# add source files
pybind11_add_module(vns_python_wrapper ${SOURCES} wrapper.cpp)

# link libraries
target_link_libraries(vns_python_wrapper PRIVATE nlohmann_json::nlohmann_json nlohmann_json_schema_validator vns_lib)
# set cmake ecn
cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(vns_python_wrapper)

# Enable Position Independent Code
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# set python version
# set(PYBIND11_PYTHON_VERSION "3.10")

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
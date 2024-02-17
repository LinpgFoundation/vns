# set cmake ecn
cmake_minimum_required(VERSION 3.25)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "C++ version selection")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(vns_python_wrapper)

# find python
find_package(Python COMPONENTS Interpreter Development)

# specify needed source files
file(GLOB SOURCES "extern/vns-cpp/libs/*/*.h" "extern/vns-cpp/libs/*/*.cpp" "extern/vns-cpp/libs/*/*.hpp" "extern/vns-cpp/*.h" "extern/vns-cpp/*.cpp")

# add pybind11
add_subdirectory(extern/pybind11)

# add source files
pybind11_add_module(vns_python_wrapper ${SOURCES} wrapper.cpp)

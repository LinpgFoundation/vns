# CMakeList.txt : CMake project for vns, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.27)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

project(vns)

file(GLOB SOURCES "libs/*/*.h" "libs/*/*.cpp" "libs/*/*.hpp")

# Add source to this project's executable.
add_executable(vns ${SOURCES} "vns.cpp" "vns.h" "scriptProcessor.cpp" "naming.cpp" "naming.h" "content.h" "content.cpp" "contentNext.h" "contentNext.cpp" "scriptProcessor.h" "functions.h" "tests.h" "tests.cpp" "functions.cpp" "compiler.h" "compiler.cpp" "version.h" "contentManager.h" "contentManager.cpp" "decompiler.cpp" "decompiler.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET vns PROPERTY CXX_STANDARD 20)
endif ()

# TODO: Add tests and install targets if needed.
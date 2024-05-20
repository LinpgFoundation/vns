# CMakeList.txt : CMake project for vns, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.25)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
    cmake_policy(SET CMP0141 NEW)
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif ()

project(vns)

# Add the json library
add_subdirectory(extern/json)

# Add the json-schema-validator library
add_subdirectory(extern/json-schema-validator)

# Add source to this project's executable.
add_executable(vns ${SOURCES} vns.cpp vns.hpp scriptProcessor.cpp naming.cpp naming.hpp dialogue.hpp dialogue.cpp
        dialogueNext.hpp dialogueNext.cpp scriptProcessor.hpp functions.hpp tests.hpp tests.cpp functions.cpp
        compiler.hpp compiler.cpp version.hpp dialoguesManager.hpp dialoguesManager.cpp decompiler.cpp decompiler.hpp
        Event.cpp Event.hpp expressionParser.cpp expressionParser.hpp number.cpp number.hpp operation.hpp
        validator.hpp validator.cpp schema.hpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET vns PROPERTY CXX_STANDARD 20)
endif ()

target_link_libraries(vns PRIVATE nlohmann_json::nlohmann_json nlohmann_json_schema_validator)

# TODO: Add tests and install targets if needed.
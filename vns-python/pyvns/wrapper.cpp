#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include "extern/vns-cpp/naming.hpp"
#include "extern/vns-cpp/compiler.hpp"
#include "extern/vns-cpp/contentManager.hpp"

namespace py = pybind11;

PYBIND11_MODULE(vns_python_wrapper, m) {

    // Add docstring for the module
    m.doc() = "Python bindings for the VNS C++ library";

    py::class_<Naming>(m, "Naming", "Character name preprocessing module")
            .def(py::init<const std::string &>())
            .def("to_string", &Naming::to_string, "Retrieve naming as string")
            .def("get_name", &Naming::get_name, "Get the name")
            .def("get_tags", &Naming::get_tags, "Get the tags associated with the name")
            .def("contains_tag", &Naming::contains_tag, "Check if the name contains a tag")
            .def("insert_tag", &Naming::insert_tag, "Insert a tag to the name")
            .def("erase_tag", &Naming::erase_tag, "Erase a tag from the name")
            .def("equal", (bool (Naming::*)(const std::string &) const) &Naming::equal,
                 "Check if the name is equal to a given naming string")
            .def("equal", (bool (Naming::*)(const std::string &, bool) const) &Naming::equal,
                 "Check if the name is equal to a given naming string, with an option to force absolute equality")
            .def("equal", (bool (Naming::*)(const Naming &) const) &Naming::equal,
                 "Check if the name is equal to another Naming object")
            .def("equal", (bool (Naming::*)(const Naming &, bool) const) &Naming::equal,
                 "Check if the name is equal to another Naming object, with an option to force absolute equality")
            .def_static("get_database", &Naming::get_database, py::return_value_policy::reference,
                        "Get the character name database")
            .def_static("clear_database", &Naming::clear_database, "Clear the character naming database")
            .def_static("update_database", (void (*)(std::string &)) &Naming::update_database,
                        "Update the character naming database from a JSON string")
            .def_static("update_database", (void (*)(
                                const std::unordered_map<std::string, std::unordered_set<std::string>> &)) &Naming::update_database,
                        "Update the character naming database from a map of names and sets of tags")
            .def_static("update_database", (void (*)(
                                const std::unordered_map<std::string, std::vector<std::string>> &)) &Naming::update_database,
                        "Update the character naming database from a map of names and vectors of tags");

    py::class_<Compiler>(m, "Compiler", "Compiler for compiling vns files")
            .def_static("load", &Compiler::load, "Load a vns file");

    py::class_<ContentNext>(m, "ContentNext", "Class for representing the next content in dialogue")
            .def(py::init<std::string, ContentNextValueType>())
            .def(py::init<const std::unordered_map<std::string, ContentNextValueType> &>(),
                 "Initialize a ContentNext object from a map")
            .def("get_type", &ContentNext::get_type, "Get the type of the next content")
            .def("get_target", &ContentNext::get_target, "Get the target of the next content")
            .def("get_targets", &ContentNext::get_targets, "Get the targets of the next content")
            .def("has_single_target", &ContentNext::has_single_target, "Check if the next content has a single target")
            .def("has_multi_targets", &ContentNext::has_multi_targets, "Check if the next content has multiple targets")
            .def("is_null", &ContentNext::is_null, "Check if the next content is null")
            .def("to_map", &ContentNext::to_map, "Convert the next object to a map");

    py::class_<Content>(m, "Content", "Class representing dialogue content")
            .def(py::init<const ContentDataType &, const std::string &>())
            .def_readwrite("previous", &Content::previous)
            .def_readwrite("next", &Content::next)
            .def_readwrite("background_image", &Content::background_image)
            .def_readwrite("background_music", &Content::background_music)
            .def_readwrite("character_images", &Content::character_images)
            .def_readwrite("contents", &Content::contents)
            .def_readwrite("narrator", &Content::narrator)
            .def_readwrite("comments", &Content::comments)
            .def_readwrite("id", &Content::id)
            .def("has_next", &Content::has_next, "Check if the content has next")
            .def("to_map", &Content::to_map, "Convert the content object to a map");

    py::class_<ContentManager>(m, "ContentManager", "Class for managing dialogue content")
            .def(py::init<>())
            .def("get_previous", &ContentManager::get_previous, py::return_value_policy::reference,
                 "Get previous content")
            .def("get_current", &ContentManager::get_current, py::return_value_policy::reference, "Get current content")
            .def("get_last", &ContentManager::get_last, py::return_value_policy::reference, "Get last content")
            .def("save", &ContentManager::save, "Save modifications to the current dialog interface")
            .def("empty", &ContentManager::empty, "Check if data is empty")
            .def("clear", &ContentManager::clear, "Clear data")
            .def("set_data", &ContentManager::set_data, "Update data")
            .def("get_data", &ContentManager::get_data, "Get data")
            .def("get_id", &ContentManager::get_id, "Get current dialogue ID")
            .def("set_id", &ContentManager::set_id, "Update current dialogue ID")
            .def("get_section", &ContentManager::get_section, "Get current section name")
            .def("set_section", &ContentManager::set_section, "Set current section name")
            .def("contains_section", &ContentManager::contains_section, "Check if dialogue has given section name")
            .def("remove_section", &ContentManager::remove_section, "Remove section")
            .def("get_current_section_contents", &ContentManager::get_current_section_contents,
                 py::return_value_policy::reference, "Get current section content")
            .def("get_section_contents", &ContentManager::get_section_contents, py::return_value_policy::reference,
                 "Get section content by name")
            .def("set_current_section_contents", &ContentManager::set_current_section_contents,
                 "Set current section content")
            .def("set_section_contents", &ContentManager::set_section_contents, "Set section content by name")
            .def("get_current_content", &ContentManager::get_current_content, py::return_value_policy::reference,
                 "Get current content data")
            .def("get_content", &ContentManager::get_content, py::return_value_policy::reference,
                 "Get content data by ID")
            .def("set_current_content", &ContentManager::set_current_content, "Set current content data")
            .def("set_content", &ContentManager::set_content, "Set content data by ID")
            .def("contains_content", &ContentManager::contains_content,
                 "Check if dialogue with given section contain given ID")
            .def("remove_current_content", &ContentManager::remove_current_content, "Remove current content data")
            .def("remove_content", &ContentManager::remove_content, "Remove content data by ID");
}
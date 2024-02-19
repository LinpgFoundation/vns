#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include "extern/vns-cpp/naming.hpp"
#include "extern/vns-cpp/compiler.hpp"
#include "extern/vns-cpp/dialoguesManager.hpp"

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

    py::class_<DialogueNext>(m, "DialogueNext", "Class for representing the dialogue next")
            .def(py::init<std::string, DialogueNextValueType>())
            .def(py::init<const std::unordered_map<std::string, DialogueNextValueType> &>(),
                 "Initialize a ContentNext object from a map")
            .def("get_type", &DialogueNext::get_type, "Get the type of the next")
            .def("get_target", &DialogueNext::get_target, "Get the target of the next")
            .def("get_targets", &DialogueNext::get_targets, "Get the targets of the next")
            .def("has_single_target", &DialogueNext::has_single_target, "Check if the next has a single target")
            .def("has_multi_targets", &DialogueNext::has_multi_targets, "Check if the next has multiple targets")
            .def("is_null", &DialogueNext::is_null, "Check if the next is null")
            .def("to_map", &DialogueNext::to_map, "Convert the next object to a map");

    py::class_<Dialogue>(m, "Dialogue", "Class representing dialogue content")
            .def(py::init<const DialogueDataType &, const std::string &>())
            .def_readwrite("previous", &Dialogue::previous)
            .def_readwrite("next", &Dialogue::next)
            .def_readwrite("background_image", &Dialogue::background_image)
            .def_readwrite("background_music", &Dialogue::background_music)
            .def_readwrite("character_images", &Dialogue::character_images)
            .def_readwrite("contents", &Dialogue::contents)
            .def_readwrite("narrator", &Dialogue::narrator)
            .def_readwrite("comments", &Dialogue::comments)
            .def_readwrite("id", &Dialogue::id)
            .def("has_next", &Dialogue::has_next, "Check if the dialogue has next")
            .def("set_next", py::overload_cast<std::string, DialogueNextValueType>(&Dialogue::set_next),
                 "Set dialogue next")
            .def("set_next",
                 py::overload_cast<const std::unordered_map<std::string, DialogueNextValueType> &>(&Dialogue::set_next),
                 "Set dialogue next")
            .def("to_map", &Dialogue::to_map, "Convert the dialogue object to a map");

    py::class_<DialoguesManager>(m, "DialoguesManager", "Class for managing dialogue contents")
            .def(py::init<>())
            .def("get_previous", &DialoguesManager::get_previous, py::return_value_policy::reference,
                 "Get previous dialogue")
            .def("get_current", &DialoguesManager::get_current, py::return_value_policy::reference,
                 "Get current dialogue")
            .def("get_last", &DialoguesManager::get_last, py::return_value_policy::reference, "Get last dialogue")
            .def("load", &DialoguesManager::load, "load dialogue data from vns file")
            .def("save", &DialoguesManager::save, "Save modifications to the current dialogue interface")
            .def("empty", &DialoguesManager::empty, "Check if data is empty")
            .def("clear", &DialoguesManager::clear, "Clear data")
            .def("set_data", &DialoguesManager::set_data, "Update data")
            .def("get_data", &DialoguesManager::get_data, "Get data")
            .def("get_id", &DialoguesManager::get_id, "Get current dialogue ID")
            .def("set_id", &DialoguesManager::set_id, "Set current dialogue ID")
            .def("get_section", &DialoguesManager::get_section, "Get current section name")
            .def("get_sections", &DialoguesManager::get_sections, "Get the names of all sections")
            .def("set_section", &DialoguesManager::set_section, "Set current section name")
            .def("contains_section", &DialoguesManager::contains_section, "Check if dialogues have given section name")
            .def("remove_section", &DialoguesManager::remove_section, "Remove section")
            .def("get_current_section_dialogues", &DialoguesManager::get_current_section_dialogues,
                 py::return_value_policy::reference, "Get current section dialogue contents")
            .def("get_section_dialogues", &DialoguesManager::get_section_dialogues, py::return_value_policy::reference,
                 "Get section dialogue contents by section name")
            .def("set_current_section_dialogues", &DialoguesManager::set_current_section_dialogues,
                 "Set current section dialogue contents")
            .def("set_section_dialogues", &DialoguesManager::set_section_dialogues,
                 "Set section dialogue contents by section name")
            .def("get_current_dialogue", &DialoguesManager::get_current_dialogue, py::return_value_policy::reference,
                 "Get current dialogue data")
            .def("get_dialogue", &DialoguesManager::get_dialogue, py::return_value_policy::reference,
                 "Get dialogue data by ID")
            .def("set_current_dialogue", &DialoguesManager::set_current_dialogue, "Set current dialogue data")
            .def("set_dialogue", &DialoguesManager::set_dialogue, "Set current dialogue data by ID")
            .def("contains_dialogue", &DialoguesManager::contains_dialogue,
                 "Check if section contains given dialogue ID")
            .def("remove_current_dialogue", &DialoguesManager::remove_current_dialogue, "Remove current dialogue")
            .def("remove_dialogue", &DialoguesManager::remove_dialogue, "Remove dialogue by ID");
}
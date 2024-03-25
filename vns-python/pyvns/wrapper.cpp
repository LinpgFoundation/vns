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
                        "Update the character naming database from a dict of names and sets of tags")
            .def_static("update_database", (void (*)(
                                const std::unordered_map<std::string, std::vector<std::string>> &)) &Naming::update_database,
                        "Update the character naming database from a dict of names and vectors of tags");

    py::class_<Compiler>(m, "Compiler", "Compiler for compiling vns files")
            .def_static("load", &Compiler::load, "Load a vns file");

    py::class_<DialogueNext>(m, "DialogueNext", "Class for representing the dialogue next")
            .def(py::init<std::string, DialogueNextValueType>())
            .def(py::init<const std::unordered_map<std::string, DialogueNextValueType> &>(),
                 "Initialize a ContentNext object from a dict")
            .def("get_type", &DialogueNext::get_type, "Get the type of the next")
            .def("has_type", &DialogueNext::has_type, "Whether the next has given type")
            .def("get_target", &DialogueNext::get_target, "Get the target of the next")
            .def("get_targets", &DialogueNext::get_targets, "Get the targets of the next")
            .def("has_single_target", &DialogueNext::has_single_target, "Check if the next has a single target")
            .def("has_multi_targets", &DialogueNext::has_multi_targets, "Check if the next has multiple targets")
            .def("is_null", &DialogueNext::is_null, "Check if the next is null")
            .def("to_dict", &DialogueNext::to_map, "Convert the next object to a dict");

    py::class_<Event>(m, "Event", "Class for representing the event struct")
            .def(py::init<std::string, std::string, EventValueType>())
            .def(py::init<EventDataType>())
            .def("to_dict", &Event::to_map)
            .def_readonly("type", &Event::type)
            .def_readonly("target", &Event::target);

    py::class_<Dialogue>(m, "Dialogue", "Class representing dialogue content")
            .def(py::init<const DialogueDataType &, const std::string &>())
            .def_readwrite("previous", &Dialogue::previous)
            .def_readwrite("next", &Dialogue::next)
            .def_readwrite("background_image", &Dialogue::background_image)
            .def_readwrite("background_music", &Dialogue::background_music)
            .def_readwrite("character_images", &Dialogue::character_images)
            .def_readwrite("contents", &Dialogue::contents)
            .def_readwrite("narrator", &Dialogue::narrator)
            .def_readwrite("notes", &Dialogue::notes)
            .def_readonly("events", &Dialogue::events)
            .def_readwrite("id", &Dialogue::id)
            .def("has_next", &Dialogue::has_next, "Check if the dialogue has next")
            .def("set_next", py::overload_cast<std::string, DialogueNextValueType>(&Dialogue::set_next),
                 "Set dialogue next")
            .def("set_next",
                 py::overload_cast<const std::unordered_map<std::string, DialogueNextValueType> &>(&Dialogue::set_next),
                 "Set dialogue next")
            .def("remove_next", &Dialogue::remove_next, "Remove dialogue next")
            .def("to_dict", &Dialogue::to_map, "Convert the dialogue object to a dict");

    py::class_<DialoguesManager>(m, "DialoguesManager", "Class for managing dialogue contents")
            .def(py::init<>())
            .def("get_previous", &DialoguesManager::get_previous, py::return_value_policy::reference,
                 "Get previous dialogue")
            .def("get_current", &DialoguesManager::get_current, py::return_value_policy::reference,
                 "Get current dialogue")
            .def("get_last", &DialoguesManager::get_last, py::return_value_policy::reference, "Get last dialogue")
            .def("load", &DialoguesManager::load, "load dialogue data from vns file")
            .def("empty", &DialoguesManager::empty, "Check if data is empty")
            .def("clear", &DialoguesManager::clear, "Clear data")
            .def("update", &DialoguesManager::update, "Update data")
            .def("next", &DialoguesManager::next, "Go to next dialogue")
            .def("contains_variable", &DialoguesManager::contains_variable, "Contains variable")
            .def("get_variable",
                 (EventValueType (DialoguesManager::*)(const std::string &) const) &DialoguesManager::get_variable,
                 "Get variable")
            .def("get_bool_variable", [](const DialoguesManager &self, const std::string &name) {
                return self.get_variable<bool>(name);
            }, "Get variable as bool")
            .def("get_int_variable", [](const DialoguesManager &self, const std::string &name) {
                return self.get_variable<int>(name);
            }, "Get variable as int")
            .def("get_float_variable", [](const DialoguesManager &self, const std::string &name) {
                return self.get_variable<float>(name);
            }, "Get variable as float")
            .def("get_str_variable", [](const DialoguesManager &self, const std::string &name) {
                return self.get_variable<std::string>(name);
            }, "Get variable as str")
            .def("set_variable", &DialoguesManager::set_variable, "Set variable")
            .def("to_dict", &DialoguesManager::to_map, "Get data in dict")
            .def("get_current_dialogue_id", &DialoguesManager::get_current_dialogue_id, "Get current dialogue ID")
            .def("set_current_dialogue_id", &DialoguesManager::set_current_dialogue_id, "Set current dialogue ID")
            .def("get_section", &DialoguesManager::get_section, "Get current section name")
            .def("get_sections", &DialoguesManager::get_sections, "Get the names of all sections")
            .def("set_section", &DialoguesManager::set_section, "Set current section name")
            .def("contains_section", &DialoguesManager::contains_section, "Check if dialogues have given section name")
            .def("remove_section", &DialoguesManager::remove_section, "Remove section")
            .def("get_current_section_dialogues", &DialoguesManager::get_current_section_dialogues,
                 py::return_value_policy::reference, "Get current section dialogue contents")
            .def("set_current_section_dialogues", &DialoguesManager::set_current_section_dialogues,
                 "Set current section dialogue contents")
            .def("set_dialogues", &DialoguesManager::set_dialogues, "Set section dialogue contents by section name")
            .def("get_dialogues", &DialoguesManager::get_dialogues, py::return_value_policy::reference,
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
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include "extern/vns-cpp/naming.hpp"
#include "extern/vns-cpp/compiler.hpp"
#include "extern/vns-cpp/contentManager.hpp"

namespace py = pybind11;

PYBIND11_MODULE(vns_python_wrapper, m) {
    py::class_<Naming>(m, "Naming")
            .def(py::init<const std::string &>())
            .def("to_string", &Naming::to_string)
            .def("get_name", &Naming::get_name)
            .def("get_tags", &Naming::get_tags)
            .def("contains_tag", &Naming::contains_tag)
            .def("insert_tag", &Naming::insert_tag)
            .def("erase_tag", &Naming::erase_tag)
            .def("equal", (bool (Naming::*)(const std::string &) const) &Naming::equal)
            .def("equal", (bool (Naming::*)(const std::string &, bool) const) &Naming::equal)
            .def("equal", (bool (Naming::*)(const Naming &) const) &Naming::equal)
            .def("equal", (bool (Naming::*)(const Naming &, bool) const) &Naming::equal)
            .def_static("get_database", &Naming::get_database, py::return_value_policy::reference)
            .def_static("clear_database", &Naming::clear_database)
            .def_static("update_database", (void (*)(std::string &)) &Naming::update_database)
            .def_static("update_database", (void (*)(
                    const std::unordered_map<std::string, std::unordered_set<std::string>> &)) &Naming::update_database)
            .def_static("update_database", (void (*)(
                    const std::unordered_map<std::string, std::vector<std::string>> &)) &Naming::update_database);

    py::class_<Compiler>(m, "Compiler")
            .def_static("load", &Compiler::load);

    py::class_<ContentNext>(m, "ContentNext")
            .def(py::init<std::string, ContentNextValueType>())
            .def(py::init<const std::unordered_map<std::string, ContentNextValueType> &>())
            .def("get_type", &ContentNext::get_type)
            .def("get_target", &ContentNext::get_target)
            .def("get_targets", &ContentNext::get_targets)
            .def("has_single_target", &ContentNext::has_single_target)
            .def("has_multi_targets", &ContentNext::has_multi_targets)
            .def("is_null", &ContentNext::is_null)
            .def("to_map", &ContentNext::to_map);

    py::class_<Content>(m, "Content")
            .def(py::init<const ContentDataType &, const std::string &>())
            .def(py::init<>()) // Default constructor
            .def_readwrite("previous", &Content::previous)
            .def_readwrite("next", &Content::next)
            .def_readwrite("background_image", &Content::background_image)
            .def_readwrite("background_music", &Content::background_music)
            .def_readwrite("character_images", &Content::character_images)
            .def_readwrite("contents", &Content::contents)
            .def_readwrite("narrator", &Content::narrator)
            .def_readwrite("comments", &Content::comments)
            .def_readwrite("id", &Content::id)
            .def("has_next", &Content::has_next)
            .def("to_map", &Content::to_map);

    py::class_<ContentManager>(m, "ContentManager")
            .def(py::init<>())
            .def("get_previous", &ContentManager::get_previous, py::return_value_policy::reference)
            .def("get_current", &ContentManager::get_current, py::return_value_policy::reference)
            .def("get_last", &ContentManager::get_last, py::return_value_policy::reference)
            .def("save", &ContentManager::save)
            .def("empty", &ContentManager::empty)
            .def("clear", &ContentManager::clear)
            .def("set_data", &ContentManager::set_data)
            .def("get_data", &ContentManager::get_data)
            .def("get_id", &ContentManager::get_id)
            .def("set_id", &ContentManager::set_id)
            .def("get_section", &ContentManager::get_section)
            .def("set_section", &ContentManager::set_section)
            .def("remove_section", &ContentManager::remove_section)
            .def("get_section_contents", (SectionDataType &(ContentManager::*)()) &ContentManager::get_section_contents,
                 py::return_value_policy::reference)
            .def("get_section_contents",
                 (SectionDataType &(ContentManager::*)(const std::string &)) &ContentManager::get_section_contents,
                 py::return_value_policy::reference)
            .def("set_section_contents",
                 (void (ContentManager::*)(const SectionDataType &)) &ContentManager::set_section_contents)
            .def("set_section_contents", (void (ContentManager::*)(const std::string &,
                                                                   const SectionDataType &)) &ContentManager::set_section_contents)
            .def("get_content", (ContentDataType &(ContentManager::*)()) &ContentManager::get_content,
                 py::return_value_policy::reference)
            .def("get_content", (ContentDataType &(ContentManager::*)(const std::string &,
                                                                      const std::string &)) &ContentManager::get_content,
                 py::return_value_policy::reference)
            .def("remove_content", (void (ContentManager::*)()) &ContentManager::remove_content)
            .def("remove_content",
                 (void (ContentManager::*)(const std::string &, const std::string &)) &ContentManager::remove_content);
}
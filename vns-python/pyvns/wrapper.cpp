#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>
#include "extern/vns-cpp/naming.hpp"
#include "extern/vns-cpp/compiler.hpp"

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
            .def_static("get_database", &Naming::get_database)
            .def_static("get_database_as_json", &Naming::get_database_as_json)
            .def_static("clear_database", &Naming::clear_database)
            .def_static("update_database", (void (*)(std::string &)) &Naming::update_database)
            .def_static("update_database", (void (*)(const std::unordered_map<std::string, std::unordered_set<std::string>> &)) &Naming::update_database)
            .def_static("update_database", (void (*)(const std::unordered_map<std::string, std::vector<std::string>> &)) &Naming::update_database);

   py::class_<Compiler>(m, "Compiler")
       .def_static("load", &Compiler::load);
}
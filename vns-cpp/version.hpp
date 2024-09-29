#ifndef VERSION_HPP
#define VERSION_HPP

#include <string>

// vns main version
constexpr size_t VERSION = 2;
// vns revision
constexpr size_t REVISION = 3;
// vns patch
constexpr size_t PATCH = 0;

// if given version is compatible
bool is_version_compatible(std::string_view, size_t, size_t);

#endif

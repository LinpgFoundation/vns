#ifndef DECOMPILER_HPP
#define DECOMPILER_HPP

#include "dialoguesManager.hpp"

class Decompiler
{
    static std::string to_str_in_case_null(const std::string &);

public:
    // tag format
    static const std::string inline TAG_STARTS = "[";
    static const std::string inline TAG_ENDS = "]";

    static void decompile(const std::unordered_map<std::string, std::any> &, const std::filesystem::path &);
};

#endif

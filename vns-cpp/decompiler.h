#ifndef DECOMPILER_H
#define DECOMPILER_H

#include "contentManager.h" // Assuming you have a header file for ContentManager

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

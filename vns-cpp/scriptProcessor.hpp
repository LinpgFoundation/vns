#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <regex>
#include "dialoguesManager.hpp"
#include "tags.hpp"
#include <sstream>

class ScriptProcessor
{
public:
    static const std::string inline SCRIPTS_FILE_EXTENSION = ".vns";
    static const std::unordered_set<std::string> inline RESERVED_WORDS = {"null",
                                                                          "none",
                                                                          "head"};
    static const std::string inline TAG_STARTS = "[";
    static const std::string inline TAG_ENDS = "]";
    static const std::string inline COMMENT_PREFIX = "//";
    static const std::string inline NOTE_PREFIX = "#";

    ScriptProcessor() : current_data_("head"), blocked_(false)
    {
    }

    [[nodiscard]] std::string get_id() const;

    [[nodiscard]] std::string get_language() const;

    void process(const std::string &);

    void process(const std::vector<std::string> &);

    void process(const std::filesystem::path &);

    [[nodiscard]] DialoguesManager get_output() const;

private:
    std::filesystem::path path_;
    DialoguesManager output_;
    Dialogue current_data_;
    std::string id_;
    std::string language_;
    std::string section_;
    std::string previous_;
    std::vector<std::string> lines_;
    std::unordered_map<size_t, std::string> dialog_associate_key_;
    std::unordered_map<std::string, std::string> branches_;

    bool blocked_;

    const static std::regex inline vns_version_pattern{R"((>=|<=|!<=|!>=|)\s*(\d+)\.(\d+))"};
    const static std::unordered_set<std::string_view> inline preprocessed_tags{tags::label, tags::vns, tags::language,
                                                                               tags::id};

    void continue_process();

    static std::string ensure_not_null(const std::string &);

    static std::string extract_parameter(const std::string &);

    static std::string_view extract_tag(const std::string &);

    static std::string extract_string(const std::string &);

    [[noreturn]] void terminated(const std::string &) const;

    [[noreturn]] void terminated(const std::string &, const size_t &) const;

    [[noreturn]] void terminated(const std::string &, const size_t &, const std::string_view &) const;

    void convert(size_t);
};

#endif

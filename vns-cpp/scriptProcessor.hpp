#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include "dialoguesManager.hpp"

class ScriptProcessor
{
public:
    static const std::string inline SCRIPTS_FILE_EXTENSION = ".vns";
    static const std::unordered_map<std::string, std::string> inline ALTERNATIVES = {{"lang", "language"},
                                                                                     {"opt",  "option"},
                                                                                     {"disp", "display"}};
    static const std::unordered_set<std::string> inline RESERVED_WORDS = {"null", "none", "head"};
    static const std::string inline TAG_STARTS = "[";
    static const std::string inline TAG_ENDS = "]";
    static const std::string inline COMMENT_PREFIX = "//";
    static const std::string inline NOTE_PREFIX = "#";

    ScriptProcessor() : current_data_("head", {}), blocked_(false)
    {
    }

    [[nodiscard]] std::string get_id() const;

    [[nodiscard]] std::string get_language() const;

    void process(const std::filesystem::path &);

    [[nodiscard]] DialoguesManager get_output() const;

private:
    std::filesystem::path path_;
    DialoguesManager output_;
    Dialogue current_data_;
    std::string id_;
    std::string lang_;
    std::string section_;
    std::string previous_;
    std::vector<std::string> lines_;
    std::unordered_map<size_t, std::string> dialog_associate_key_;

    bool blocked_;

    static std::string ensure_not_null(const std::string &);

    static std::string extract_parameter(const std::string &);

    static std::string extract_tag(const std::string &);

    static std::string extract_string(const std::string &);

    [[noreturn]] void terminated(const std::string &) const;

    [[noreturn]] void terminated(const std::string &, const size_t &) const;

    void convert(size_t);
};

#endif

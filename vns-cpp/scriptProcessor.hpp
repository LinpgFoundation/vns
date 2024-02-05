#ifndef PROCESSOR_HPP
#define PROCESSOR_HPP

#include <unordered_map>
#include <unordered_set>
#include <filesystem>
#include "content.hpp"

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
    static const std::string inline NOTE_PREFIX = "#";
    static const std::string inline COMMENT_PREFIX = "//";

    ScriptProcessor() : line_index_(0), current_data_({}, "head"), blocked_(false)
    {
    }

    [[nodiscard]] std::string get_id() const;

    [[nodiscard]] std::string get_language() const;

    void process(const std::filesystem::path &);

    [[nodiscard]] DialogueDataType get_output() const;

    [[nodiscard]] nlohmann::json get_output_as_json() const;

private:
    std::filesystem::path path_;
    size_t line_index_;
    std::unordered_map<std::string, std::unordered_map<std::string, Content> > output_;
    Content current_data_;
    std::string id_;
    std::string lang_;
    std::string section_;
    std::string previous_;
    std::vector<std::string> lines_;
    std::unordered_map<size_t, std::string> dialog_associate_key_;
    std::vector<std::string> accumulated_comments_;
    bool blocked_;

    static std::string ensure_not_null(const std::string &);

    static std::string extract_parameter(const std::string &);

    static std::string extract_tag(const std::string &);

    static std::string extract_string(const std::string &);

    [[noreturn]] void terminated(const std::string &) const;

    [[noreturn]] void preprocess_terminated(const std::string &) const;

    [[nodiscard]] std::string get_current_line() const;

    void convert(int);
};

#endif

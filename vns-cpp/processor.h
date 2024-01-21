#ifndef PROCESSOR_H
#define PROCESSOR_H
#include <any>
#include <unordered_map>
#include <unordered_set>

#include "content.h"

class Processor
{
public:
	static const std::string inline SCRIPTS_FILE_EXTENSION = ".vns";
	static const std::unordered_map<std::string, std::string> inline ALTERNATIVES = {
		{"lang", "language"}, {"opt", "option"}, {"disp", "display"}
	};
	static const std::unordered_set<std::string> inline RESERVED_WORDS = {"null", "none", "head"};
	static const std::string inline TAG_STARTS = "[";
	static const std::string inline TAG_ENDS = "]";

	Processor() : line_index_(0), current_data_({}, "head"), id_(-1), blocked_(false)
	{
	}

	int get_id() const;
	std::string get_language() const;
	void process(const std::string&);
	std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::any>>>
	get_output() const;

private:
	std::string path_in_;
	int line_index_;
	std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::any>>> output_;
	Content current_data_;
	int id_;
	std::string lang_;
	std::string section_;
	std::string previous_;
	std::vector<std::string> lines_;
	std::unordered_map<int, std::string> dialog_associate_key_;
	std::vector<std::string> accumulated_comments_;
	bool blocked_;
	static std::string ensure_not_null(const std::string&);
	static std::string extract_parameter(const std::string&);
	static std::string extract_tag(const std::string&);
	static std::string extract_string(const std::string&);
	void terminated(const std::string&) const;
	std::string get_current_line() const;
	// void convert(int);
};
#endif

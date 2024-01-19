#ifndef CONTENT_NEXT_H
#define CONTENT_NEXT_H
#include <string>
#include <vector>
#include <unordered_map>
#include <string>
#include <variant>
#include <vector>

using multi_targets_type = std::vector<std::unordered_map<std::string, std::string>>;

using content_next_value_type = std::variant<std::string, multi_targets_type>;

class ContentNext
{
public:
	ContentNext(const std::unordered_map<std::string, content_next_value_type>&);
	ContentNext();
	std::string get_type() const;
	std::string get_target() const;
	multi_targets_type get_targets() const;
	bool has_multi_targets() const;
	bool is_null() const;
	std::unordered_map<std::string, content_next_value_type> to_map() const;

private:
	std::string type_;
	content_next_value_type target_;
};

#endif

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
	ContentNext(const std::string& type, const content_next_value_type& target) : type_(type), target_(target)
	{
	}

	ContentNext(const std::unordered_map<std::string, content_next_value_type>& data) : ContentNext(
		std::get<std::string>(data.at("type")), data.at("target"))
	{
	}

	ContentNext() : ContentNext("null", "")
	{
	}

	[[nodiscard]] std::string get_type() const;
	[[nodiscard]] std::string get_target() const;
	[[nodiscard]] multi_targets_type get_targets() const;
	[[nodiscard]] bool has_multi_targets() const;
	[[nodiscard]] bool is_null() const;
	[[nodiscard]] std::unordered_map<std::string, content_next_value_type> to_map() const;

private:
	std::string type_;
	content_next_value_type target_;
};

const ContentNext kNullContentNext;
#endif

#include "contentNext.h"


ContentNext::ContentNext(const std::unordered_map<std::string, content_next_value_type>& data)
{
	type_ = std::get<std::string>(data.at("type"));
	target_ = data.at("target");
}

ContentNext::ContentNext()
{
	type_ = "default";
	target_ = "";
}

std::unordered_map<std::string, content_next_value_type> ContentNext::to_map() const
{
	return {
		{"type", type_}, {"target", target_}
	};
}

bool ContentNext::has_multi_targets() const
{
	return !std::holds_alternative<std::string>(target_);
}

std::string ContentNext::get_type() const
{
	return type_;
}


std::string ContentNext::get_target() const
{
	return std::get<std::string>(target_);
}

multi_targets_type ContentNext::get_targets() const
{
	return std::get<multi_targets_type>(target_);
}


bool ContentNext::is_null() const
{
	if (type_.empty())
	{
		return true;
	}
	if (std::holds_alternative<std::string>(target_))
	{
		return std::get<std::string>(target_).empty();
	}
	return std::get<multi_targets_type>(target_).empty();
}

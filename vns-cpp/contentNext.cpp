#include "contentNext.h"

std::unordered_map<std::string, ContentNextValueType> ContentNext::to_map() const
{
	return {{"type", type_}, {"target", target_}};
}

nlohmann::json ContentNext::to_json() const
{
	nlohmann::json json_data;
	if (!is_null())
	{
		json_data["type"] = type_;
		if (has_multi_targets())
		{
			json_data["target"] = get_targets();
		}
		else
		{
			json_data["target"] = get_target();
		}
	}
	return json_data;
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

MultiTargetsType ContentNext::get_targets() const
{
	return std::get<MultiTargetsType>(target_);
}


bool ContentNext::is_null() const
{
	if (type_.empty() || type_ == "null")
	{
		return true;
	}
	if (std::holds_alternative<std::string>(target_))
	{
		return std::get<std::string>(target_).empty();
	}
	return std::get<MultiTargetsType>(target_).empty();
}

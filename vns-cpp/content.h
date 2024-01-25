#ifndef CONTENT_H
#define CONTENT_H
#include <string>
#include <vector>
#include <unordered_map>
#include <string>
#include <vector>
#include "contentNext.h"

using ContentValueType = std::variant<std::string, std::vector<std::string>, std::unordered_map<
	                                      std::string, ContentNextValueType>>;

struct Content
{
	Content(const std::unordered_map<std::string, ContentValueType>&, const std::string&);

	Content() : Content({}, "head")
	{
	}

	[[nodiscard]] bool has_next() const;
	[[nodiscard]] std::unordered_map<std::string, ContentValueType> to_map() const;
	[[nodiscard]] nlohmann::json to_json() const;
	std::string previous;
	ContentNext next;
	std::string background_image;
	std::string background_music;
	std::vector<std::string> character_images;
	std::vector<std::string> contents;
	std::string narrator;
	std::vector<std::string> comments;
	std::string id;

	template <typename T>
	static T cast(const std::unordered_map<std::string, ContentValueType>& data, const std::string& k, T default_v)
	{
		const auto it = data.find(k);
		if (it != data.end())
		{
			return std::get<T>(it->second);
		}
		return default_v;
	}
};

#endif

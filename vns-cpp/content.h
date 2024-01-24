#ifndef CONTENT_H
#define CONTENT_H
#include <any>
#include <string>
#include <vector>
#include <unordered_map>
#include <string>
#include <vector>
# include "contentNext.h"

struct Content
{
	Content(const std::unordered_map<std::string, std::any>&, const std::string&);

	Content() : Content({}, "head")
	{
	}

	[[nodiscard]] bool has_next() const;
	[[nodiscard]] std::unordered_map<std::string, std::any> to_map() const;
	std::string previous;
	ContentNext next;
	std::string background_image;
	std::string background_music;
	std::vector<std::string> character_images;
	std::vector<std::string> contents;
	std::string narrator;
	std::vector<std::string> comments;
	std::string id;
};

#endif

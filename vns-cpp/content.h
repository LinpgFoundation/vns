#ifndef CONTENT_H
#define CONTENT_H
#include <any>
#include <string>
#include <vector>
#include <unordered_map>
#include <string>
#include <vector>
# include "contentNext.h"

class Content
{
public:
	Content(const std::unordered_map<std::string, std::any>&, const std::string&);
	std::string id() const;
	bool has_next() const;
	std::unordered_map<std::string, std::any> to_map() const;
	std::string previous_;
	ContentNext next_;
	std::string background_image_;
	std::string background_music_;
	std::vector<std::string> character_images_;
	std::vector<std::string> contents_;
	std::string narrator_;
	std::vector<std::string> comments_;

private:
	std::string id_;
};

#endif

#include "content.h"


Content::Content(const std::unordered_map<std::string, std::any>& data, const std::string& content_id)
{
	id = content_id;
	background_image = std::any_cast<std::string>(data.at("background_image"));
	background_music = std::any_cast<std::string>(data.at("background_music"));
	character_images = std::any_cast<std::vector<std::string>>(data.at("character_images"));
	contents = std::any_cast<std::vector<std::string>>(data.at("contents"));
	narrator = std::any_cast<std::string>(data.at("narrator"));
	previous = std::any_cast<std::string>(data.at("previous"));
	next = ContentNext(std::any_cast<std::unordered_map<std::string, content_next_value_type>>(data.at("next")));
	comments = std::any_cast<std::vector<std::string>>(data.at("comments"));
}

bool Content::has_next() const { return !next.is_null(); }

std::unordered_map<std::string, std::any> Content::to_map() const
{
	return {
		{"background_image", background_image},
		{"background_music", background_music},
		{"character_images", character_images},
		{"contents", contents},
		{"previous", previous},
		{"narrator", narrator},
		{"next", next.to_map()},
		{"comments", comments}
	};
}

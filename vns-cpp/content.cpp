#include "content.h"


Content::Content(const std::unordered_map<std::string, std::any>& data, const std::string& id)
{
	id_ = id;
	background_image_ = std::any_cast<std::string>(data.at("background_image"));
	background_music_ = std::any_cast<std::string>(data.at("background_music"));
	character_images_ = std::any_cast<std::vector<std::string>>(data.at("character_images"));
	contents_ = std::any_cast<std::vector<std::string>>(data.at("contents"));
	narrator_ = std::any_cast<std::string>(data.at("narrator"));
	previous_ = std::any_cast<std::string>(data.at("previous"));
	next_ = ContentNext(std::any_cast<std::unordered_map<std::string, content_next_value_type>>(data.at("next")));
	comments_ = std::any_cast<std::vector<std::string>>(data.at("comments"));
}

std::string Content::id() const { return id_; }
bool Content::has_next() const { return !next_.is_null(); }

std::unordered_map<std::string, std::any> Content::to_map() const
{
	return {
		{"background_image", background_image_},
		{"background_music", background_music_},
		{"character_images", character_images_},
		{"contents", contents_},
		{"previous", previous_},
		{"narrator", narrator_},
		{"next", next_.to_map()},
		{"comments", comments_}
	};
}

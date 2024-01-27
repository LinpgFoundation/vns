#include "naming.h"
#include <ranges>
#include <sstream>

// Constructor
Naming::Naming(const std::string& the_name)
{
	size_t found = the_name.find('&');
	name_ = the_name.substr(0, found);

	while (found != std::string::npos)
	{
		const size_t pos = found + 1;
		found = the_name.find('&', pos);
		tags_.insert(the_name.substr(pos, found - pos));
	}
}

// Get combined name and tags as a string
std::string Naming::to_string() const
{
	std::stringstream result;
	result << name_;
	for (const auto& tag : tags_)
	{
		result << "&" << tag;
	}
	return result.str();
}

// Accessor for name
std::string Naming::get_name() const
{
	return name_;
}

// Accessor for tags
std::unordered_set<std::string> Naming::get_tags() const
{
	return tags_;
}

// If contains a tag
bool Naming::contains_tag(const std::string& tag) const
{
	return tags_.contains(tag);
}

// Insert a tag
void Naming::insert_tag(const std::string& tag)
{
	tags_.insert(tag);
}

// Erase a tag
void Naming::erase_tag(const std::string& tag)
{
	if (tags_.contains(tag))
	{
		tags_.erase(tag);
	}
}

// Check if two Naming objects or a Naming object and a string refer to the same character
bool Naming::equal(const std::variant<Naming, std::string>& o, const bool must_be_the_same) const
{
	const Naming other = std::holds_alternative<std::string>(o)
		                     ? Naming(std::get<std::string>(o))
		                     : std::get<Naming>(o);
	if (name_ == other.get_name())
	{
		return true;
	}

	if (!must_be_the_same)
	{
		for (const std::vector<std::string>& value : std::views::values(DATABASE_))
		{
			if (std::ranges::find(value.begin(), value.end(), name_) != value.end())
			{
				return std::ranges::find(value.begin(), value.end(), other.get_name()) != value.end();
			}
		}
	}
	return false;
}

// update database
void Naming::update_database(const std::unordered_map<std::string, std::vector<std::string>>& database)
{
	for (const auto& pair : database)
	{
		DATABASE_[pair.first] = pair.second;
	}
}

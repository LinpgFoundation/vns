#include "naming.h"

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
std::string Naming::ToString() const
{
	std::string result = name_;
	for (const auto& tag : tags_)
	{
		result += "&" + tag;
	}
	return result;
}

// Accessor for name
std::string Naming::GetName() const
{
	return name_;
}

// Accessor for tags
std::unordered_set<std::string> Naming::GetTags() const
{
	return tags_;
}

// Check if two Naming objects or a Naming object and a string refer to the same character
bool Naming::Equal(const std::variant<Naming, std::string>& o, const bool must_be_the_same) const
{
	const Naming other = std::holds_alternative<std::string>(o)
		                     ? Naming(std::get<std::string>(o))
		                     : std::get<Naming>(o);
	if (name_ == other.GetName())
	{
		return true;
	}

	if (!must_be_the_same)
	{
		for (const auto& [key, value] : DATABASE_)
		{
			auto it = std::ranges::find(value.begin(), value.end(), name_);
			if (it != value.end())
			{
				return std::ranges::find(value.begin(), value.end(), other.GetName()) != value.end();
			}
		}
	}
	return false;
}

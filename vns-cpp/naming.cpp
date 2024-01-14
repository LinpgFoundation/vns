#include "naming.h"

// Constructor
Naming::Naming(const string& the_name)
{
	size_t pos = 0;
	size_t found = the_name.find("&");
	name_ = the_name.substr(0, found);

	while (found != string::npos)
	{
		pos = found + 1;
		found = the_name.find("&", pos);
		tags_.insert(the_name.substr(pos, found - pos));
	}
}

// Get combined name and tags as a string
string Naming::ToString() const
{
	string result = name_;
	for (const auto& tag : tags_)
	{
		result += "&" + tag;
	}
	return result;
}

// Accessor for name
string Naming::GetName() const
{
	return name_;
}

// Accessor for tags
unordered_set<string> Naming::GetTags() const
{
	return tags_;
}

// Check if two Naming objects or a Naming object and a string refer to the same character
bool Naming::Equal(const Naming& other, bool must_be_the_same = false) const
{
	if (name_ == other.GetName())
	{
		return true;
	}
	/*
	if (!mustBeTheSame) {
	    for (const auto& [key, value] : DATABASE_) {
	        auto it = find(value.begin(), value.end(), name_);
	        if (it != value.end()) {
	            return find(value.begin(), value.end(), other.GetName()) != value.end();
	        }
	    }
	}*/
	return false;
}

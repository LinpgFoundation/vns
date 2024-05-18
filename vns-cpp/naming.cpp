#include "naming.hpp"
#include <ranges>
#include <sstream>
#include <nlohmann/json.hpp>

// Constructor
Naming::Naming(const std::string &the_name)
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
    for (const auto &tag: tags_)
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
bool Naming::contains_tag(const std::string &tag) const
{
    return tags_.contains(tag);
}

// Insert a tag
void Naming::insert_tag(const std::string &tag)
{
    tags_.insert(tag);
}

// Erase a tag
void Naming::erase_tag(const std::string &tag)
{
    if (tags_.contains(tag))
    {
        tags_.erase(tag);
    }
}

// Check if two Naming objects or a Naming object and a string refer to the same character
bool Naming::equal(const std::string &other) const
{
    return equal(Naming(other), false);
}

bool Naming::equal(const std::string &other, const bool must_be_the_same) const
{
    return equal(Naming(other), must_be_the_same);
}

bool Naming::equal(const Naming &other) const
{
    return equal(other, false);
}

bool Naming::equal(const Naming &other, const bool must_be_the_same) const
{
    if (name_ == other.get_name())
    {
        return true;
    }

    if (!must_be_the_same)
    {
        for (const std::unordered_set<std::string> &value: std::views::values(DATABASE_))
        {
            if (std::ranges::find(value.begin(), value.end(), name_) != value.end())
            {
                return std::ranges::find(value.begin(), value.end(), other.get_name()) != value.end();
            }
        }
    }
    return false;
}

// access the database
std::unordered_map<std::string, std::unordered_set<std::string>> &Naming::get_database()
{
    return DATABASE_;
}

void Naming::clear_database()
{
    return DATABASE_.clear();
}

// update database
void Naming::update_database(std::string &database_str)
{
    const std::unordered_map<std::string, std::vector<std::string>> database = nlohmann::json::parse(database_str);
    update_database(database);
}

void Naming::update_database(const std::unordered_map<std::string, std::unordered_set<std::string>> &database)
{
    for (const auto &[k, v]: database)
    {
        DATABASE_[k] = v;
    }
}

void Naming::update_database(const std::unordered_map<std::string, std::vector<std::string>> &database)
{
    for (const auto &[k, v]: database)
    {
        DATABASE_[k] = std::unordered_set<std::string>(v.begin(), v.end());
    }
}

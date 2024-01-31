#ifndef NAMING_H
#define NAMING_H

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

// Character name preprocessing module
class Naming
{
    std::string name_;
    std::unordered_set<std::string> tags_;
    inline static std::unordered_map<std::string, std::vector<std::string>> DATABASE_;

public:
    Naming(const std::string &);

    [[nodiscard]] std::string to_string() const;

    [[nodiscard]] std::string get_name() const;

    [[nodiscard]] std::unordered_set<std::string> get_tags() const;

    [[nodiscard]] bool contains_tag(const std::string &) const;

    void insert_tag(const std::string &);

    void erase_tag(const std::string &);

    [[nodiscard]] bool equal(const std::string &, bool must_be_the_same = false) const;

    [[nodiscard]] bool equal(const Naming &, bool must_be_the_same = false) const;

    static std::unordered_map<std::string, std::vector<std::string>> &get_database();

    static std::string get_database_as_json();

    static void update_database(std::string &);

    static void update_database(const std::unordered_map<std::string, std::vector<std::string>> &);
};

#endif

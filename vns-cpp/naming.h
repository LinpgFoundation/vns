#ifndef NAMING_H
#define NAMING_H
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <vector>

// 角色立绘名称预处理模块
class Naming
{
	std::string name_;
	std::unordered_set<std::string> tags_;
	inline static std::unordered_map<std::string, std::vector<std::string>> DATABASE_ = {};

public:
	Naming(const std::string&);
	[[nodiscard]] std::string ToString() const;
	[[nodiscard]] std::string GetName() const;
	[[nodiscard]] std::unordered_set<std::string> GetTags() const;
	// Class method to access the database
	static std::unordered_map<std::string, std::vector<std::string>>& GetDatabase()
	{
		return DATABASE_;
	}

	[[nodiscard]] bool Equal(const std::variant<Naming, std::string>&, bool must_be_the_same = false) const;
};
#endif

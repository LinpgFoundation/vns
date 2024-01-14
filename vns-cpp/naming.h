#ifndef NAMING_H_
#define NAMING_H_
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
using namespace std;

// 角色立绘名称预处理模块
class Naming
{
	string name_;
	unordered_set<string> tags_;
	static unordered_map<string, vector<string>> DATABASE_;

public:
	Naming(const string&);
	string ToString() const;
	string GetName() const;
	unordered_set<string> GetTags() const;
	// Class method to access the database
	static unordered_map<string, vector<string>>& GetDatabase()
	{
		return DATABASE_;
	}
	bool Equal(const Naming&, bool) const;
};
#endif

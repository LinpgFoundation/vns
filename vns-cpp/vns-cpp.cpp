// vns-cpp.cpp : Defines the entry point for the application.
//

#include "vns-cpp.h"
#include "naming.h"
#include "contentNext.h"

using namespace std;

int main()
{
	cout << "test tags:" << endl;
	Naming testName("maria.png&silent&hide");
	cout << testName.GetName() << endl;
	cout << testName.ToString() << endl;
	cout << "\ntages:" << endl;
	for (std::string v : testName.GetTags())
	{
		cout << v << endl;
	}
	cout << "\ntest empty tag next:" << endl;
	ContentNext testNext;
	cout << testNext.is_null() << endl;
	cout << testNext.has_multi_targets() << endl;
	cout << testNext.get_type() << endl;
	cout << testNext.get_target() << endl;

	cout << "\ntest not empty single target next:" << endl;
	testNext = ContentNext({{"type","default"}, {"target", "~1"}});
	cout << testNext.is_null() << endl;
	cout << testNext.has_multi_targets() << endl;
	cout << testNext.get_type() << endl;
	cout << testNext.get_target() << endl;

	cout << "\ntest not empty multiple targets next:" << endl;
	unordered_map<std::string, std::string> t1 = { {"hello1", "world1"} };
	unordered_map<std::string, std::string> t2 = { {"hello2", "world2"} };
	multi_targets_type target_v = { t1, t2};
	testNext = ContentNext({ {"type","default"}, {"target", target_v} });
	cout << testNext.is_null() << endl;
	cout << testNext.has_multi_targets() << endl;
	cout << testNext.get_type() << endl;
	for (auto dia : testNext.get_targets())
	{
		for (const auto& [k, v] : dia)
		{
			cout << k << endl;
			cout << v << endl;
		}
	}
	return 0;
}

// vns-cpp.cpp : Defines the entry point for the application.
//

#include "vns-cpp.h"
#include "naming.h"

using namespace std;

int main()
{
	Naming testName("maria.png&silent&hide");
	cout << testName.GetName() << endl;
	cout << testName.ToString() << endl;
	cout << "\ntages:" << endl;
	for (auto v : testName.GetTags())
	{
		cout << v << endl;
	}
	return 0;
}

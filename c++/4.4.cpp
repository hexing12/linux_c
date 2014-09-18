#include <iostream>
using namespace std;

int main()
{
	char *name;

	name = new char[20];
	cout << "please input your name: ";
	cin >> name;
	cout << "your name's : " << name << endl;
	delete []name;

	return 0;
}

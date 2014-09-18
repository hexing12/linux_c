#include <iostream>
#include <string.h>
#include "student.h"

using namespace std;

int main()
{
	int numberi;
	char namei[15];
	float scorei;

	cout << "Enter number,name, score: ";
	cin >> numberi >> namei >> scorei;
	Student stu1(numberi, namei, scorei);
	stu1.print();

	return 0;
}

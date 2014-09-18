#include <iostream>
#include "student.h"
#include <string.h>

using namespace std;

int main()
{
	Student stu1(1, "bill", 87);
	stu1.print();
	
	Student stu2(2, "adam", 98);
	stu2.print();

	Student stu3(3, "ghh", 45);
	stu3.print();

	return 0;
}

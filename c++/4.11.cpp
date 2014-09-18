#include <iostream>
#include <string.h>
#include "student.h"

using namespace std;

int main()
{
	Student:: init();
	Student stu1(1, "bill", 87);
	stu1.print();

	Student stu2(2, "fgdhj", 89);
	stu2.print();
	
	Student stu3(3, "dff", 67);
	stu3.print();

	Student:: average();

	return 0;
}

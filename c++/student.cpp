#include <iostream>
#include <string.h>
#include "student.h"

using namespace std;

int Student:: count;
float Student:: sum;

Student:: Student(int number1, const char *name1, float score1)
{
	number = number1;
	name = new char[strlen(name1)+1];
	strcpy(name, name1);
	score = score1;
	count++;
	sum += score;
}

Student:: ~Student()
{
	delete []name;
	count--;
}

void Student:: print()
{
	cout << "number: " << number << endl;
	cout << "name: " << name << endl;
	cout << "score: " << score << endl;
}

float Student:: average()
{
	cout << "sum is " << sum << endl;
	cout << "count is " << count << endl;
	cout << "average is " << sum/count << endl;

	return sum/count;
}

#include <iostream>
#include <string.h>

using namespace std;

class Student
{
	private:
	int number;
	char *name;
	float score;
	static int count;
	static float sum;
	
	public:
	Student(int number1, const char *name1, float score1);
	~Student();
	void modify(float score1)
	{	score = score1;
	}
	void print();
	static void init()
	{
		Student:: count = 0;
		Student:: sum = 0;
	}
	static float average();
};


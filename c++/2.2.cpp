#include <iostream>
using namespace std;

void sqr_it(int *i);

int main()
{
	int x = 10;
	sqr_it(&x);
	cout << "The square of x is " << x << endl;

	return 0;
}

void sqr_it(int *i)
{
	*i = (*i) * (*i);
}

#include <iostream>
using namespace std;

int max(int x, int y);
long max(long x, long y);
double max(double x, double y);

int main()
{
	int x1 = 1, y1 = 2;
	long x2 = 2, y2 = 3;
	double x3 = 4, y3 = 5;

	cout << "max1 = " << max(x1, y1) << endl;
	cout << "max2 = " << max(x2, y2) << endl;
	cout << "max3 = " << max(x3, y3) << endl;

}

int max(int x, int y)
{
	if(x > y) return x;
	else	  return y;
}
long max(long x, long y)
{
        if(x > y) return x;
        else      return y;
}

double max(double x, double y)
{
        if(x > y) return x;
        else      return y;
}


#include <iostream>
#include <math.h>

using namespace std;

class point
{
	public:
	point(double xi, double yi) {x = xi; y = yi;}
	double Getx() {return x;}
	double Gety() {return y;}
	friend double Distance(point &a, point &b);

	private:
	double x, y;
};

double Distance(point &a, point &b)
{
	double dx = a.Getx() - b.Gety();
	double dy = a.Getx() - b.Gety();
	
	return sqrt(dx*dx + dy*dy);
}

int main()
{
	point p1(3.0, 5.0), p2(4.0, 6.0);

	double d = Distance(p1, p2);
	cout << "The distance is " << d << endl;

	return 0;	
}

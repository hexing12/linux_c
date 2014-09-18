#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

class timer
{
private:	int seconds;
public:		timer(char *t) {seconds = atoi(t);}
		timer(int t = 10) {seconds = t;}
		timer(int min, int sec) {seconds = min*60 + sec;}
		void run();
};

void timer:: run()
{
	clock_t t1;
	t1 = clock();

	while((clock() / CLOCKS_PER_SEC - t1 / CLOCKS_PER_SEC) < seconds);
	cout << "\a";
}

int main()
{
	timer timer1(5), timer2("20");
	timer1.run();
	timer2.run();

	return 0;
}

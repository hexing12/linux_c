#include <iostream>
using namespace std;

int model = 90;
int v[] = {1, 2, 3, 4};

void f()
{
	model = 200;
	v[2]++;
}

int main()
{	int i;
	
	//修改前
	cout << "model = " << model << endl;
	for(i = 0; i < 4; i++) 
		cout << v[i] << endl;
	
	f();
	//修改后
	cout << "model = " << model << endl;
        for(i = 0; i < 4; i++) 
                cout << v[i] << endl;
	return 0;
}

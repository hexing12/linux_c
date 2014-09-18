#include <iostream>

using namespace std;

class control
{
	static int resource;
	public:
	int get_resource();
	void free_resource()
	{
		resource = 0;
	}
};

int control:: get_resource()
{	
	//资源已经被占用
	if(resource) return 0;
	
	//资源未被占用
	else{
		resource = 1;
		return 1;
	}
}

int control:: resource = 0;

int main()
{
	control ob1, ob2;
	
	if(ob1.get_resource())
		cout << "ob1 has resource\n";
	if(!ob2.get_resource())
		cout << "ob2 denied by resource\n";
	ob1.free_resource();
	if(ob2.get_resource())
		cout << "ob2 can use resource now.\n";

	return 0;
}

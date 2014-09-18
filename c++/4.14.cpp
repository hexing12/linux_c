#include <iostream>

using namespace std;

class node
{
	public:	node(int d, node *n = 0)
		{
			data = d;
			next = n;
		}
	friend class list;
		private: int data;
			 node *next;
};
	class list{
		public:  list()	{head = tail = 0; node_count = 0;}
			 ~list() {
				while(head) {
					node *tmp = head;
					head = head->next;
					delete tmp;
				}
			}
			void append(int i)
			{
				if(node_count == 0){
					node *n = new node(i);
					head = tail = n;
				}else{
					node *n = new node(i);
					tail->next = n;
					tail = n;
				}
				node_count++;
				tail->next = 0;
			}
			void show()
			{
				node *p = head;
				while(p) {
					cout << p->data << endl;
					p = p->next;
				}
			}
		private: node *head, *tail;
			 int node_count;
};

int main()
{
	list Lst;
	Lst.append(1);
	Lst.append(6);
	Lst.append(8);
	
	cout << "The data stored in the list are: \n";
	Lst.show();

	return 0;
}

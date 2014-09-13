//尾插法，以s为首结点
#include <stdio.h>
#include <stdlib.h>

#define LEN 4
#define DELENODE 2

typedef struct node
{	
	int num;
	struct node *next;
}Node;

//创建链表，同时得到要删除的结点指针
Node *Create_linklist();

//删除指定的结点
Node *Delete_node(Node *pre, Node *s);

//寻找指定结点的前驱
Node *Search_node(Node *s);

//打印链表
void Print_linklist(Node *s);

int main()
{
	Node *s, *pre, *p;

	s = Create_linklist();
	Print_linklist(s);
	p = Search_node(s);
	pre = Search_node(p);
	s = Delete_node(pre, s);
	Print_linklist(s);

	return 0;
		
}

Node *Create_linklist()
{
	Node *p, *r, *s, *head;
	int i = 1;

	head = p = (Node *)malloc(sizeof(Node));
	printf("请输入数据元素：");
	scanf("%d", &p->num);
	while(i < LEN) {
		if(i == DELENODE) {
			s = p;
		}
		r = (Node *)malloc(sizeof(Node));
		printf("请输入数据元素：");
		scanf("%d", &r->num);
		p->next = r;
		p = r;
		i++;
	}
	p->next = head; 
	return s;
}

Node *Search_node(Node *s)
{
	Node *pre = s;
	
	while(pre->next != s) 
		pre = pre->next;
	return pre;
}

Node *Delete_node(Node *pre, Node *s)
{
	Node *p = pre->next;

	pre->next = s;
	free(p);
	
	return s;
}

void Print_linklist(Node *s)
{
	Node *p = s;
	
	printf("数据元素为：");
	while(p->next != s ) {
		printf("%d\t", p->num);
		p = p->next;
	}
	printf("%d\n", p->num);
}

#include <stdio.h>
#include <stdlib.h>

typedef struct node
{
	int num;
	struct node *next;
}Lnode;

//创建链表
Lnode *Create_linklist(int n);

//合并链表
Lnode *Merge(Lnode *la, Lnode *lb);

//打印链表
void Print_linklist(Lnode *head);

int main()
{
	int ma, nb;
	Lnode *heada, *headb, *headc;

	printf("请输入链表la lb 的长度: ");
	scanf("%d%d", &ma, &nb);
	
	printf("请输入la的数据元素：");
	heada = Create_linklist(ma);
	printf("链表la的数据元素为：");
	Print_linklist(heada);
	printf("请输入lb的数据元素：");
	headb = Create_linklist(nb);
	printf("链表lb的数据元素：");
	Print_linklist(headb);

	if(ma < nb) 
		headc = Merge(heada, headb);
	else
		headc = Merge(headb, heada);
	printf("合并后的链表数据：");
	Print_linklist(headc); 
}

Lnode *Create_linklist(int n)
{
	Lnode *head, *p, *s;
	int i;

	head = p = (Lnode *)malloc(sizeof(Lnode));
	for(i = 1; i <= n; i++) {
		s = (Lnode *)malloc(sizeof(Lnode));
		scanf("%d", &s->num);
		p->next = s;
		p = s;
	}
	p->next = NULL;

	return head;
}

void Print_linklist(Lnode *head)
{
	Lnode *p = head->next;
	
	while(p) {
		printf("%d ", p->num);
		p = p->next;
	}
	printf("\n");
}

Lnode *Merge(Lnode *la, Lnode *lb)
{
	Lnode *headc = la, *p = la;
	Lnode *qa = la->next, *qb = lb->next;

	while(qa && qb) {
		p->next = qa;
		p = qa;
		qa = qa->next;
		p->next = qb;
		p = qb;
		qb = qb->next;
	}
	p = qb->next;
	free(lb);
	return headc;
}

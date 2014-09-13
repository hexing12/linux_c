//已知线性表A的长度为n，并采用顺序存储结构编写算法，删除线性表中所有值的x
#include <stdio.h>
#include <stdlib.h>

#define MAXLEN 100

typedef struct
{
	int elem[MAXLEN];
	int length;
}Seqlist;

//初始化线性表
Seqlist *Init_seqlist(int n);

//删除线性表中的x元素
Seqlist *Delete_seqlist(Seqlist *l, int x);

//输出线性表
void Print_seqlist(Seqlist *l, int n);

int main()
{
	Seqlist *l;
	int n, x;
	
	printf("请输入线性表的实际长度：");
	scanf("%d", &n);
	l = Init_seqlist(n);
	printf("原始数据: ");
	Print_seqlist(l, n);
	printf("请输入要删除的元素: ");
	scanf("%d", &x);
	l = Delete_seqlist(l, x);
	if(!l->length) {
		printf("删除后为空表!\n");
		return 0;
	} else {
		printf("删除后的数据: ");
		Print_seqlist(l, l->length);
	}
	return 0;
}

Seqlist *Init_seqlist(int n)
{
	Seqlist *l;
	l = (Seqlist *)malloc(sizeof(Seqlist));
	int i = 0;

	for(i = 0; i < n; i++) {
		printf("请输入数组元素：");
		scanf("%d", &l->elem[i]);
	}
	l->length = n;
	return l;
}

void Print_seqlist(Seqlist *l, int n)
{
	int i;

	for(i = 0; i < n; i++) {
		printf("%d  ", l->elem[i]);
	}
	printf("\n");
}

Seqlist *Delete_seqlist(Seqlist *l, int x)
{
	int i, j;

	if(l->length == 0) {
		printf("抱歉，链表为空!\n");
		exit(1);
	}
	for(i = 0; i < l->length; i++) {
		if(l->elem[i] == x) {
			l->length--;
			for(j = i; j <= l->length-1; j++)
				l->elem[j] = l->elem[j+1];
			i = -1;
		}
	}
	return l;
}

遇见的问题： 指针的初始化
	     删除后的下标问题
	     删除后为空的细节

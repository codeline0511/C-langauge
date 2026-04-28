#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef int element;
typedef struct DlistNode
{
	element data;
	struct DlistNode* llink;
	struct DlistNode* rlink;
} DlistNode;

void init(DlistNode* phead)
{
	phead->llink = phead;
	phead->rlink = phead;
}

void print_dlist(DlistNode* phead)
{
	DlistNode* p;
	for (p = phead->rlink; p!=phead; p = p->rlink)
	{
		printf("<-| |%d| |->", p->data);
	}
	printf("\n");
}

void dinsert(DlistNode* before, element data)
{
	DlistNode* newnode = (DlistNode*)malloc(sizeof(DlistNode));
	strcpy(newnode->data, data); // 정의되지 않음
	newnode->llink = before;
	newnode->rlink = before->rlink;
	before->llink->rlink = newnode;
	before->rlink = newnode;
}

void ddelete(DlistNode* head, DlistNode* removed)
{
	if (removed == head) return;
	removed->llink->rlink = removed->rlink;
	removed->rlink->llink = removed->llink;
	free(removed);
}

int main(void)
{
	DlistNode* head = (DlistNode*)malloc(sizeof(DlistNode));
	init(head);
	printf("추가 단계\n");
	for (int i = 0; i < 5; i++)
	{
		dinsert(head, i);
		print_dlist(head);
	}
	printf("삭제 단계\n");
	for (int i = 0; i < 5; i++)
	{
		print_dlist(head);
		ddelete(head, head->rlink);
	}
	free(head);
	return 0;
}
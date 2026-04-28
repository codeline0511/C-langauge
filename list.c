#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef int element;
typedef struct ListNode {
	element data;
	struct ListNode* link;
} ListNode;


ListNode* insert_first(ListNode* head, element data)
{
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	node->data = data;
	if (head == NULL) {
		head = node;
		node->link = head;
	}
	else {
		node->link = head->link; 
		head->link = node; 
	}
	return head;
}

ListNode* insert_last(ListNode* head, element data)
{
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	node->data = data;
	if (head == NULL) {
		head = node;
		node->link = head;
	}
	else {
		node->link = head->link; 
		head->link = node; 
		head = node;
	}
	return head;
}

ListNode* insert(ListNode* head, ListNode* pre, element data) // Á» ´õ Å½±¸ ¿ä¸Á
{
	ListNode* node = (ListNode*)malloc(sizeof(ListNode));
	node->data = data;
	if (head == NULL) {
		head = node;
		node->link = head;
	}
	else if (pre == head) {
		node->link = pre->link;
		pre->link = node;
		head = node;
	}
	else
	{
		node->link = pre->link;
		pre->link = node;
	}
	return head; 
}

ListNode* delete_first(ListNode* head)
{
	if (head == NULL)
		return NULL;
	if (head->link == head) {
		free(head);
		head = NULL;
	}
	else {
		ListNode* removed = head->link;
		head->link = removed->link;
		free(removed);
	}
	return head;
}

ListNode* delete(ListNode* head, ListNode* pre)
{
	ListNode* removed;
	if (head == NULL)
		return NULL;
	if (pre->link == pre) {
		free(pre);
		head = NULL;
	}
	else {
		if (pre->link == head)
			head = pre;
		removed = pre->link;
		pre->link = removed->link;
		free(removed);
	}
	return head;
}

ListNode* delete_last(ListNode* head)
{
	ListNode* removed, * pre;
	if (head == NULL)
		return NULL;
	if (head == head->link) {
		removed = head;
		head = NULL;
	}
	else {
		pre = head->link;
		while (pre->link != head)
			pre = pre->link;
		removed = head;
		pre->link = head->link;
		head = pre;
	}
	free(removed);
	return head;
}

void print_list(ListNode* head)
{
	ListNode* p;
	if (head == NULL) return;
	p = head->link;
	while (1) {
		printf("%d->", p->data);
		if (p == head)
			break;
		p = p->link;
	}
}

int main(void)
{
	ListNode* head = NULL;
	head = insert_last(head, 20);
	head = insert_last(head, 30);
	head = insert_last(head, 40);
	head = insert_first(head, 10);
	print_list(head);
	return 0;
}
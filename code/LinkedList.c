#include <stdio.h>
#include <stdlib.h>
#include "LinkedList.h"

LinkedList* createLinkedList()
{	
	LinkedList *list;

	list = (LinkedList*)malloc(sizeof(LinkedList));
	list->head = NULL;
	return list;
}

void insertNodeEnd(LinkedList *list, int a,int b,int r,int f, char pid)
{
	LinkedListNode *newNode = NULL, *tempNode = NULL;
	
	newNode = (LinkedListNode*)malloc(sizeof(LinkedListNode));
	newNode->next = NULL;
	newNode->a = a;
	newNode->b = b;
	newNode->r = r;
	newNode->f = f;
	newNode->pid = pid;
	
	tempNode = list->head;
	/*Check if the list is empty*/	
	if(list->head == NULL)
	{
		list->head = newNode;
	}
	/*If it is not, then itterate over the list*/
	else{
	while( (tempNode->next) != NULL)
	{
		tempNode = tempNode->next;
	}

	tempNode->next = newNode;
	}
}

LinkedListNode* removeFirstElement(LinkedList *list)
{
	LinkedListNode *temp = NULL;
	
	temp = list->head;
	list->head = temp->next;
	return temp; 
}

int getLength(LinkedList *list)
{

	int length = 0;
	LinkedListNode *current;
	
	current = list->head;
	while(current != NULL) {
		length++;
		current = (*current).next;
	}
	return length;
}


void freeLinkedList(LinkedList *list) 
{
	LinkedListNode *node, *nextNode;
	node = list->head;
	while(node != NULL) {
		nextNode = (node->next);
		free(node);
		node = nextNode;
	}
	list->head=NULL;
}


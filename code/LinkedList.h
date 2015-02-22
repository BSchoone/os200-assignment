#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_


/********************************
* LinkedListNode Struct
* Contains a pointer to another linked list node and a void pointer to some sort of data
********************************/
typedef struct LinkedListNode { int a; int b; int r; int f; char pid; struct LinkedListNode *next;} LinkedListNode;

/********************************
* LinkedList Struct
* Contains a pointer to a linked list node
********************************/

typedef struct LinkedList { LinkedListNode *head; } LinkedList;

LinkedList* createLinkedList();
void insertNodeEnd(LinkedList *, int,int,int,int, char );
LinkedListNode* removeFirstElement(LinkedList *);
int getLength(LinkedList *);
void freeLinkedList(LinkedList*);

#endif

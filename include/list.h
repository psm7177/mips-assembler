#ifndef _LIST
#define _LIST
#include <stdbool.h>

typedef struct LinkedList LinkedList;
typedef struct LinkedListNode LinkedListNode;

struct LinkedList{
    LinkedListNode *head;
    LinkedListNode *tail;
};

struct LinkedListNode
{
    void* value;
    LinkedListNode *next;
    // void *(void)(void *p_token_node) deallocate;
};

LinkedList*  init_linkedlist();
void append(LinkedList* list, void* value);
void extend(LinkedList* list, LinkedList* elements);
void insert(LinkedList* list, LinkedListNode* before ,void* value);
void* pop(LinkedList* list);
void remove_iter(LinkedList* list, LinkedListNode* from, int count);
void remove_item(LinkedList* list, LinkedListNode* item);
void destructure(LinkedList *list);
LinkedListNode* iter(LinkedList *list);
LinkedListNode* next(LinkedListNode* node);
bool is_empty(LinkedList* list);

#endif
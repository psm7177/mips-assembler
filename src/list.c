#include "list.h"
#include <stddef.h>
#include <stdlib.h>

LinkedList* init_linkedlist()
{
    LinkedList* list = malloc(sizeof(LinkedList));
    list->head = NULL;
    list->tail = NULL;
    
    return list;
};

void append(LinkedList *l, void *value)
{
    LinkedListNode *element = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    element->value = value;
    element->next = NULL;

    if (is_empty(l))
    {
        l->head = element;
        l->tail = element;
    }
    else
    {
        l->tail->next = element;
        l->tail = l->tail->next;
    }

    return;
}
void extend(LinkedList* list, LinkedList* elements){
    while(is_empty(elements)){
        LinkedListNode * e = pop(elements);
        append(list,e->value);
        free(e);
    }
    return;
}

void insert(LinkedList* list, LinkedListNode* before ,void* value)
{
    LinkedListNode *element = (LinkedListNode *)malloc(sizeof(LinkedListNode));
    element->value = value;
    element-> next = NULL;

    if(before->next == NULL){
        element->next = before->next;
        before->next = element;
        list->tail = element;
    } else {
        element->next = before->next;
        before->next = element;
    }

    return;
}

void* pop(LinkedList *list)
{
    void *val = list->head->value;
    LinkedListNode *dealloc = list->head;

    list->head = list->head->next;

    free(dealloc);
    return val;
}
void remove_iter(LinkedList* list, LinkedListNode* from, int count){
    LinkedListNode * current;
    
    int i = 0;
    for(; current!=NULL && i < count; i++ ){
        free(current->value);
        current = next(current);
    }
    
    if(current == NULL){
        from->next = NULL;
        list->tail = from;
    } else if(i == count){
        from->next = current;
    }
}

void remove_item(LinkedList* list, LinkedListNode* item){
    LinkedListNode * current = iter(list);

}

LinkedListNode *iter(LinkedList *list){
    return list->head;
}

LinkedListNode *next(LinkedListNode *node){
    if(node->next ==NULL)
    {
        return NULL;
    }
    return node->next;
}

void destructure(LinkedList *list){
    while(!is_empty(list)){
        char * element = pop(list);
        free(element);
    }
    free(list);
}

bool is_empty(LinkedList *list)
{
    return list->head == NULL;
}

#ifndef _SYNTAX
#define _SYNTAX

#include "tree.h"
#include "list.h"
#include "binary.h"

Tree* analysis_syntax(LinkedList *list);
LinkedListNode* analysis_Directive(int section,LinkedListNode * current, binary_t *binary);
LinkedListNode* analysis_Data(int section, LinkedListNode * current, binary_t *binary);
LinkedListNode* analysis_Text(int section, LinkedListNode * current, binary_t *binary);
LinkedListNode* analysis_Word(int section, LinkedListNode * current, binary_t *binary);

LinkedListNode* analysis_PreLabel(int section, LinkedListNode * current, binary_t *binary);
LinkedListNode* analysis_Instruction(int section, LinkedListNode * current, binary_t *binary);

bool hierachy(token_t* head, token_t *current);

#endif
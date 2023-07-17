#ifndef _PSEUDO_INSTRUCTION
#define _PSEUDO_INSTRUCTION

#include "instruction.h"
#include "token.h"
#include "tree.h"
#define NUM_PSEUDO_INSTRUCTION 1

typedef struct pseudo_instruction pseudo_instruction_t;
typedef void (*convert_pseudo_instruction_t)(LinkedList *list,LinkedListNode *p_token_node);

struct pseudo_instruction
{
    char *name;
    convert_pseudo_instruction_t func;
};
void convert_LA(LinkedList *list, LinkedListNode *p_token_node);

static pseudo_instruction_t pseudo_instruction_set[1] = {
    {"la",
     convert_LA}};

void process_pseudo_instruction(int depth, Tree *tree);
void convert_pseudo_instruction(LinkedList *list, LinkedListNode *p_token_node);

#endif

#ifndef _TOKEN
#define _TOKEN

#include "list.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

enum TokenType {
    Directive,
    OffsetRegister,
    Register,
    PreLabel,
    Hex,
    PseudoInstruction,
    Instruction,
    Label,
    Dec,
    Root
};

typedef struct token
{
    enum TokenType type;
    void *value;
} token_t;

enum DirectiveType{
    DATA,
    TEXT,
    WORD
};

LinkedList *make_token(FILE *ptr);
token_t *create_token(enum TokenType t);
bool is_printable(char ch);
bool is_space(char ch);

int get_directive_type(token_t * token);
int* flatten(LinkedList* list);
#endif
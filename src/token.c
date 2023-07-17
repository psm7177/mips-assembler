#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <regex.h>
#include "parser.h"
#include "tree.h"
#include "label.h"

LinkedList *make_token(FILE *f_ptr)//TODO tokenizer
{
    char ch;
    fseek(f_ptr, 0L, SEEK_END);
    int sz = ftell(f_ptr);
    fseek(f_ptr, 0, SEEK_SET);

    LinkedList *queue = init_linkedlist();
    char buff[1024];
    char *buff_ptr = buff;

    do
    {
        ch = fgetc(f_ptr);
        if (is_printable(ch) && !is_space(ch))
        {
            *buff_ptr = ch;
            buff_ptr++;
        }
        else
        {
            if (buff != buff_ptr)
            {
                *buff_ptr = '\0';
                buff_ptr++;

                size_t size = buff_ptr - buff;
                void *value = malloc(size);
                memcpy(value, buff, size);
                append(queue, value);

                buff_ptr = buff;
            }
        }

    } while (ch != EOF);

    regex_t reg;
    size_t nmatch = 9;
    regmatch_t matches[9];

    for (int i = 0; i < nmatch; i++)
    {
        matches[i].rm_so = -1;
        matches[i].rm_eo = -1;
    }

    LinkedList *token_queue = init_linkedlist();

    while (!is_empty(queue))
    { 
        
        char *element = pop(queue);
        void* value = parse_token(element);
        append(token_queue, value);

        free(element);
    }
    free(queue);

    return token_queue;
}

token_t *create_token(enum TokenType t)
{
    token_t *token = (token_t *)malloc(sizeof(token_t));

    token->type = t;

    return token;
} 

bool is_printable(char ch)
{
    return 0x20 <= ch && ch <= 0x7e;
}

bool is_space(char ch)
{
    return ch == ' ';
}

int get_directive_type(token_t * token){
    if(token->type == Directive){
        return *((int *)(token->value));
    }
    return -1;
};

int* flatten(LinkedList* list){
    int* f = malloc(sizeof(int)*3);
    int i = 0; 
    for(LinkedListNode* current = iter(list); current != NULL; current = next(current)){
        Tree* tree = current->value;
        token_t* token = tree->token;
        switch (token->type)
        {
            case Dec:
            case Hex:
            case Register:
                f[i++] = *(int*)(token->value);
                break;
            case OffsetRegister:
                f[i++] = *(int*)(token->value);
                printf("offset: %d\n",*(int*)(token->value + 4));
                f[i++] = *(int*)(token->value + 4);
                break;
            case Label:
                label_t * label = token->value;
                if(label->address){
                    f[i++] =  label->address;
                } else {
                    f[i++] = label->group->address;
                }
                break;
        default:
            break;
        }
    }
    printf("flattend: %d\n",i);
    return f;
}

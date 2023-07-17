#include "syntax.h"
#include "token.h"
#include "binary.h"
#include "label.h"
#include "pseudo-instruction.h"
#include "tree.h"
#include <stdbool.h>

Tree *analysis_syntax(LinkedList *list)
{
    printf("analysis\n");

    LinkedListNode *current = iter(list);

    Tree* stack[16];
    Tree** head = stack;

    token_t* root_token = create_token(Root);
    Tree* root = init_Tree(root_token);
    stack[0] = root;

    while(current != NULL && head >= stack){
        token_t* token = current->value;
        if(hierachy((*head)->token, token)){
            head++;
            *head = init_Tree(token);
            current = next(current);
            printf("stack\n");
        }
        else {
            Tree** parent = head - 1;
            append((*parent)->children, *head);
            head--;
            printf("unstack\n");

        }
    }
    while (head > stack)
    {
        Tree** parent = head - 1;
        append((*parent)->children, *head);
        head--;
        printf("unstack\n");
    }
    
    postorder(0, root);

    return root;
}

bool hierachy(token_t* head, token_t *current){
    // printf("%d %d ",head->type, current->type);
    if(head->type == Root){
        printf("it is in root\n");
    }
    int directive;
    switch (head->type)
    {
    case Root:
        directive= get_directive_type(current);
        printf("%d ",directive);
        return directive == DATA || directive == TEXT;
    case Directive:
        directive = get_directive_type(head);
        switch (directive)
        {
            case DATA:
            case TEXT:
                return current->type == PreLabel;
            case WORD:
                return current->type == Dec || current->type == Hex;
            default:
                printf("Syntax Error: not declared directive.\n");
                exit(1);
        }
    case PreLabel:
        return current->type == Instruction || current->type == PseudoInstruction || get_directive_type(current) == WORD;
    case Instruction:
    case PseudoInstruction:
        return current->type == OffsetRegister || current->type == Label || current->type == Register || current->type == Dec || current->type == Hex;
    }
    return false;
}

// LinkedListNode *analysis_Directive(int section, LinkedListNode *current, binary_t *binary)
// {
//     token_t *token = current->value;
//     int directive = *(int *)(token->value);
//     printf("directive: %d\n", directive);
//     if (directive == DATA)
//     {
//         return analysis_Data(DATA, current, binary);
//     }
//     else if (directive == TEXT)
//     {
//         return analysis_Text(TEXT, current, binary);
//     }
//     else if (directive == WORD)
//     {
//         return analysis_Word(section, current, binary);
//     }

//     printf("syntax error: it is declared section.");
//     exit(1);
// }

// LinkedListNode *analysis_Data( int section, LinkedListNode *current, binary_t *binary)
// {
//     for(current = next(current); ((token_t*)(current->value))->type == PreLabel;){
//         printf("--prelabel--\n");
//         current = analysis_PreLabel(section, current, binary);
//         printf("token type: %d\n", ((token_t*)(current->value))->type);
//     }
//     return current;
// }

// LinkedListNode *analysis_Text(int section, LinkedListNode *current, binary_t *binary)
// {
//     for(current = next(current); ((token_t*)(current->value))->type == Instruction ;){
//         return analysis_PreLabel(section, current, binary);
//     }
// }

// LinkedListNode *analysis_Word( int section, LinkedListNode *current, binary_t *binary)
// {
//     current = next(current);
//     token_t *token = current->value;

//     if (token->type == Dec || token->type == Hex)
//     {
//         printf("word value: %d\n", *((int *)(token->value)));
//     }
//     return current;
// }

// LinkedListNode *analysis_PreLabel(int section, LinkedListNode *current, binary_t *binary)
// {
//     token_t *token = current->value;
//     LabelGroup_t *group = token->value;
//     printf("group:\n");
//     for(int i = 0; i < num_label_group; i ++){
//         LabelGroup_t *labelgroup = label_group + i;
//         printf("\t%s\n", labelgroup->name);
//     }
//     printf("prelabel: %s\n", group->name);
//     if (section == DATA)
//     {
//         apply_address(group->children, binary->offset_data);
//         current = next(current);
//         token_t* token = current->value;
//         while(token ->value == Directive){

//         }
//         for(;((token_t*)(current->value))->type == Directive ;current=next(current)){
//            current = analysis_Directive(section,current,binary);
//         }
//         return current;
//     }
//     else if (section == TEXT)
//     {
//         apply_address(group->children, binary->offset_text);

//         return current;
//     }
//     printf("syntax error: it is declared section.");
//     exit(1);
// }


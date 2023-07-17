#include "pseudo-instruction.h"
#include "token.h"
#include "label.h"
#include "tree.h"
#include <string.h>

void convert_LA(LinkedList *list, LinkedListNode *p_instruction_node)
{
    printf("la convert\n");
    LinkedListNode *p_instrunction_token_node = p_instruction_node;
    Tree *p_instruction = p_instruction_node->value;
    token_t* p_instruction_token= get_token(p_instruction_node);

    LinkedListNode * instruction_children = iter(p_instruction->children);
    token_t *p_register_token = get_token(instruction_children);

    instruction_children = next(instruction_children);
    token_t *p_address_token = get_token(instruction_children);

    char *p_pseudo_instruction = p_instruction_token->value;

    free(p_pseudo_instruction);
    p_instruction_token->type = Instruction;
    p_instruction_token->value = malloc(sizeof(char) * 4);
    memcpy(p_instruction_token->value, "lui", 4);

    label_t *label = (label_t *)p_address_token->value;
    int address = (int)label->address;
    int upper = address >> 16;
    int lower = (address << 16) >>16;
    
    free(p_address_token->value);
    p_address_token->type = Dec;
    p_address_token->value = malloc(sizeof(int));
    memcpy(p_address_token->value, &upper, sizeof(int));
    printf("address: 0x%x upper: 0x%x lower: 0x%x\n", address,upper,lower);
    if(address == 0){
        printf("address don't have not NULL.\n");
        exit(1);
    }
    if (lower != 0)
    {
        p_instruction_token = create_token(Instruction);
        token_t * cpy_p_register_token = create_token(Register);
        p_address_token = create_token(Dec);

        p_instruction = init_Tree(p_instruction_token);
        Tree * p_register = init_Tree(cpy_p_register_token);
        Tree * p_address = init_Tree(p_address_token);
        // instruction 
        p_instruction_token->value = malloc(sizeof(char) * 4);
        memcpy(p_instruction_token->value, "ori", 4);

        // register
        cpy_p_register_token->value = p_register_token->value;
        // address
        p_address_token->value = malloc(sizeof(int));
        memcpy(p_address_token->value, &lower, sizeof(int));
        
        // append instruction 
        insert(list,p_instrunction_token_node,p_instruction);
        // append register and address
        append(p_instruction->children,p_register);
        append(p_instruction->children,p_register);
        append(p_instruction->children,p_address);
    }
    return;
}

void process_pseudo_instruction(int depth, Tree *tree)
{
    if(depth == 2){
        for(LinkedListNode* current = iter(tree->children); current != NULL; current = next(current)){
            TreeNode * node = (current->value);
            if(node->token->type == PseudoInstruction){
                convert_pseudo_instruction(tree->children, current);
            }
        }
        return;
    }
    for(LinkedListNode* current = iter(tree->children); current != NULL; current = next(current)){
        TreeNode * node = (current->value);
        process_pseudo_instruction(depth + 1, node);
    }
}

void convert_pseudo_instruction(LinkedList *list, LinkedListNode * tree_node)
{
    Tree * tree = tree_node->value;
    token_t *p_token = tree->token;
    printf("%d in convert_pseudo_instruction\n", p_token->type);
    char *name = (char *)p_token->value;
    for (int i = 0; i < NUM_PSEUDO_INSTRUCTION; i++)
    {
        pseudo_instruction_t *converter = pseudo_instruction_set + i;\
        printf("%s, %s\n",converter->name, name);
        if (strcmp(converter->name, name) == 0)
        {
            return converter->func(list, tree_node);
        }
    }
}

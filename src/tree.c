#include "tree.h"
#include "binary.h"
#include <stdlib.h>

Tree* init_Tree(token_t * token){
    Tree*  root = init_TreeNode(token);
    root->children = init_linkedlist();
    return root;
}

TreeNode* init_TreeNode(token_t * token){
    TreeNode* root = (TreeNode*) malloc(sizeof(TreeNode));
    root->children = NULL;
    root->token = token;
    return root;
}

token_t* get_token(LinkedListNode* node){
    Tree *tree_node = node->value;
    return tree_node->token;
}

int postorder(int depth, Tree * tree){
    for(int i = 0; i < depth;i++){
        printf("\t");
    }
    printf("%d \n", tree->token->type);
    for(LinkedListNode* current = iter(tree->children); current != NULL; current = next(current)){
        TreeNode * node = (current->value);
        postorder(depth + 1, node);
    }
}
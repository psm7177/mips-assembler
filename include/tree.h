#ifndef _TREE
#define _TREE

#include "list.h"
#include "token.h"

typedef struct TreeNode TreeNode;
typedef struct TreeNode Tree;

struct TreeNode{
    token_t * token;
    LinkedList* children;
};

Tree* init_Tree(token_t * token);
TreeNode* init_TreeNode(token_t * token);
token_t* get_token(LinkedListNode* node);
int postorder(int depth, Tree * tree);
#endif
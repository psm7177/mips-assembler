#ifndef _LABEL
#define _LABEL

#include "list.h"
#include "binary.h"
typedef struct Label label_t;
typedef struct LabelGroup LabelGroup_t;

struct Label{
    char * name;
    LabelGroup_t* group;
    int address;
};

struct LabelGroup
{
    char * name;
    LinkedList* children;
    int address;
};

label_t* create_label(char * name);
void put_label(label_t * label);
LabelGroup_t* add_label_group(char* label);
LabelGroup_t* get_label_group(char * name);

void apply_address(LinkedList * group, int address);
void set_label(Tree* text_tree);
void view_label_group();
extern int num_label_group;
extern LabelGroup_t label_group[32];

#endif
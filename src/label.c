#include "label.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include "binary.h"

int num_label_group = 0;
LabelGroup_t label_group[32];

label_t* create_label(char * name){
    label_t *label = malloc(sizeof(label_t));
    label->name = name;
    label->group = NULL;
    label->address = 0;
    return label;
}

void put_label(label_t * label){
    LabelGroup_t* group;
    for(int i = 0; i < num_label_group;i++){
        group = label_group + i;
        if(strcmp(group->name, label->name) == 0){
            append(group->children,label);
            label->group = group;
            return;
        }
    }
    group = add_label_group(label->name);
    append(group->children,label);
    label->group = group;
    return;
}

LabelGroup_t* add_label_group(char* name){
    LabelGroup_t * group = label_group + num_label_group;
    int length = strlen(name)+1;
    group->name = malloc(sizeof(char)*length);
    memcpy(group->name, name, length);
    group->children = init_linkedlist();
    group->address = 0;
    num_label_group++;
    return group;
}

LabelGroup_t* get_label_group(char *name){
    for(int i = 0; i < num_label_group;i++){
        LabelGroup_t* group = label_group + i;
        if(strcmp(group->name, name) == 0){
            return group;
        }
    }

    return add_label_group(name);
}

void apply_address(LinkedList * group, int address){
    for(LinkedListNode* current = iter(group); current != NULL; current = next(current)){
        label_t* label = current->value;
        label->address = address;
    }
}
void set_label(Tree* text_tree){
    LinkedList *text_children = text_tree->children;
    int offset = TEXT_SECTION_OFFSET;
    for(LinkedListNode* current_label =  iter(text_children); current_label != NULL; current_label = next(current_label)){
        Tree * text_prelabel = current_label->value;
        LabelGroup_t* group = text_prelabel->token->value;
        group->address = offset;
        apply_address(group->children, offset);
        for(LinkedListNode* current_instruction =  iter(text_prelabel->children); current_instruction != NULL; current_instruction = next(current_instruction)){
            offset += SECTION_SIZE;
        }
    }
}

void view_label_group(){
    for(int i = 0; i < num_label_group; i ++){
        LabelGroup_t *labelgroup = label_group + i;
    }
}

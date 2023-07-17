#include <stdio.h>
#include "token.h"
#include "parser.h"
#include "syntax.h"
#include "pseudo-instruction.h"
#include "binary.h"
#include "label.h"

char *parse_path(int argc, char *argv[]);
char * convert_obj(char * path);

int main(int argc, char *argv[])
{
    init_parser();
    char *path = parse_path(argc, argv);
    FILE *p_comp_file;
    p_comp_file = fopen(path, "rt");

    if (p_comp_file == NULL)
    {
        printf("file can't be open");
        exit(1);
    }

    LinkedList *token_queue = make_token(p_comp_file);

    Tree* root = analysis_syntax(token_queue);

    LinkedList* root_children = root->children;
    LinkedListNode* p_child = iter(root_children);
    Tree* data_tree = p_child->value;
    p_child = next(p_child);
    Tree* text_tree = p_child->value;


    binary_t binary = {};
    binary.data = init_linkedlist();
    binary.text = init_linkedlist();
    binary.offset_text = TEXT_SECTION_OFFSET;
    binary.offset_data = DATA_SECTION_OFFSET;

    write_data(&binary,data_tree);
    process_pseudo_instruction(0,root);
    set_label(text_tree);
    write_text(&binary,text_tree);
    
    postorder(0, root);
    printf("text_offset: %d\n", binary.offset_text);
    printf("data_offset: %d\n", binary.offset_data);

    FILE *p_out_file = fopen(convert_obj(path),"wt");
    fprintf(p_out_file, "0x%x\n",binary.offset_text - TEXT_SECTION_OFFSET);
    fprintf(p_out_file, "0x%x\n",binary.offset_data - DATA_SECTION_OFFSET);
    for(LinkedListNode * current = iter(binary.text); current != NULL; current= next(current)){
        fprintf(p_out_file,"0x%x\n", *(int*)(current->value));
    }
    for(LinkedListNode * current = iter(binary.data); current != NULL; current= next(current)){
        fprintf(p_out_file,"0x%x\n", *(int*)(current->value));
    }
    fclose(p_comp_file);
    fclose(p_out_file);
    return 0;
}

char *parse_path(int argc, char *argv[])
{
    if (argc == 0)
    {
        printf("need file to compile.");
        exit(1);
    }
    else
    {
        return argv[1];
    }
}

char * convert_obj(char * path){
    int length = strlen(path) + 1;
    char * crop_filename = malloc(sizeof(char)*length);
    memcpy(crop_filename, path,length);
    crop_filename[length - 2] = 'o';
    return crop_filename;
}
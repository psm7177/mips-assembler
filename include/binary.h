#ifndef _BINARY
#define _BINARY

#include "list.h"
#include "tree.h"

#define TEXT_SECTION_OFFSET 0x00400000
#define DATA_SECTION_OFFSET 0x10000000
#define SECTION_SIZE 4

typedef struct Binary binary_t;

struct Binary{
    int offset_text;
    int offset_data;
    LinkedList* text;
    LinkedList* data;
};

void write_binary(int section, binary_t * binary, int _4_byte);

void write_data(binary_t * binary, Tree* tree);
void write_data_prelabel(binary_t * binary, Tree* tree);
int get_word_value(Tree* tree);

void write_text(binary_t * binary, Tree* tree);
void write_text_prelabel(binary_t * binary, Tree* tree);
void write_instruction(binary_t * binary, Tree* tree);
unsigned int mask_bit(int offset, int size);

#endif
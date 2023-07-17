#include "binary.h"
#include "token.h"
#include "label.h"
#include "instruction.h"

void write_binary(int section, binary_t * binary, int _4_byte){
    int *line = malloc(sizeof(int));
    *line = _4_byte;
    if(section == DATA){
        append(binary->data, line);
        binary->offset_data += SECTION_SIZE;
    }
    if(section == TEXT){
        append(binary->text,line);
        binary->offset_text += SECTION_SIZE;
    }
}

void write_data(binary_t * binary, Tree* tree){
    for(LinkedListNode* current =  iter(tree->children); current != NULL; current = next(current)){
        Tree * data_prelabel = current->value;
        LabelGroup_t* group = data_prelabel->token->value;
        group->address = binary->offset_data;
        apply_address(group->children, binary->offset_data);
        write_data_prelabel(binary, data_prelabel);
    }
}

void write_data_prelabel(binary_t * binary, Tree* tree){
    for(LinkedListNode* current =  iter(tree->children); current != NULL; current = next(current)){
        Tree * word = current->value;
        int line = get_word_value(word);
        printf("data: %d\n",line);
        write_binary(DATA, binary, line);
    }
}

int get_word_value(Tree* tree){
    LinkedListNode * word_node = iter(tree->children);
    Tree* value_tree = word_node->value;
    return *(int*)(value_tree->token->value);
}

void write_text(binary_t * binary, Tree* tree){
    for(LinkedListNode* current =  iter(tree->children); current != NULL; current = next(current)){
        Tree * text_prelabel = current->value;
        write_text_prelabel(binary, text_prelabel);
    }
}

void write_text_prelabel(binary_t * binary, Tree* tree){
    for(LinkedListNode* current =  iter(tree->children); current != NULL; current = next(current)){
        Tree * inst = current->value;
        write_instruction(binary, inst);
    }
}

void write_instruction(binary_t * binary, Tree* tree){
    token_t* token = tree->token;
    
    for(int i = 0; i < NUM_INSTRUCTION; i++){
        instruction_t backbone = insturction_set[i];
        unsigned int line = 0;
        if(strcmp(backbone.name, token->value) == 0){
            printf("%s\n",backbone.name);
            int* flat= flatten(tree->children);
            unsigned int opcode_mask = mask_bit(0, 6);
            unsigned int opcode = backbone.opcode << (32 - 6);
            line = (opcode_mask & opcode) | (~opcode_mask & line);
            printf("\topcode:\t%032b\n",line);

            int count;

            switch(backbone._type){
                case R:
                    count = 5;
                    break;
                case I:
                    count = 3;
                    break;
                case J:
                    count = 1;
                    break;
            }
            for(int j = 0; j < count;j++){
                    field_t f = backbone.field[j];
                    field_t field_info = field_set[f._type];
                    unsigned int value;

                    if(f._default == -1){
                        value = flat[j];
                    } else {
                        value = f._default;
                    }
                    if(f._type == LABEL){
                        value = (short)(value- binary->offset_text - 4)/4;
                    }

                    if(f._type == TARGET){
                        value = value/4;
                    }

                    printf("%d\n\tvalue:\t%032b\n",value,value);
                    int mask = mask_bit(field_info.offset, field_info.size);
                    
                    value = value << (32 - field_info.size);
                    value = value >> field_info.offset;
                    printf("\tshift:\t%032b\n",value);
                    line = (mask & value) | (~mask & line);
                    printf("\tline:\t%032b\n",line);

                    
            }
            write_binary(TEXT, binary, line);
        }
    }
}

unsigned int mask_bit(int offset, int size){
    unsigned int bit = 0xFFFFFFFF;
    bit = bit << 32 - size;
    bit = bit >> offset;
    printf("\tmask:\t%032b\n",bit);
    return bit;
};


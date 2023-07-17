#include <string.h>
#include <regex.h>
#include "parser.h"
#include "pseudo-instruction.h"
#include "instruction.h"
#include "label.h"

;
void *parse_Directive(regmatch_t *matches, char word[])
{
    int *p_directive = (int *)malloc(sizeof(int));
    if (strcmp(".data", word) == 0)
    {
        *p_directive = DATA;
        return p_directive;
    }
    if (strcmp(".text", word) == 0)
    {
        *p_directive = TEXT;
        return p_directive;
    }
    if (strcmp(".word", word) == 0)
    {
        *p_directive = WORD;
        return p_directive;
    }

    printf("Syntax Error: %s is not directive\n", word);
    exit(1);
}

void *parse_Register(regmatch_t *matches, char word[])
{
    int *p_register = (int *)malloc(sizeof(int));
    char *str_register = crop_match(matches[1], word);

    *p_register = atoi(str_register);
    free(str_register);
    return p_register;
}

void *parse_OffsetRegister(regmatch_t *matches, char word[])
{
    int *p_register = (int *)malloc(2 * sizeof(int));
    char *str_offset = crop_match(matches[1], word);
    char *str_register = crop_match(matches[2], word);

    p_register[0] = atoi(str_offset);
    p_register[1] = atoi(str_register);
    free(str_offset);
    free(str_register);
    return p_register;
}

void *parse_PreLabel(regmatch_t *matches, char word[])
{
    char *label = crop_match(matches[1], word);
    LabelGroup_t *group =  get_label_group(label);
    printf("%s %s\n",label,group->name);
    return group;
}

void *parse_PseudoInstruction(regmatch_t *matches, char word[])
{
    int size = strlen(word) + 1;
    char *p_pseudoInstruction = malloc(sizeof(char) * size);
    memcpy(p_pseudoInstruction, word, size);
    return p_pseudoInstruction;
}

void *parse_Instruction(regmatch_t *matches, char word[])
{
    int size = strlen(word) + 1;
    char *p_Instruction = malloc(sizeof(int) * size);
    memcpy(p_Instruction, word, size);
    return p_Instruction;
}

void *parse_Hex(regmatch_t *matches, char word[])
{
    char *hex = crop_match(matches[1], word);
    int *p_hex_value = (int *)malloc(sizeof(int));
    *p_hex_value = strtol(hex, NULL, 16);
    printf("word: %s -> parse_hex: %d\n", hex, *p_hex_value);
    return p_hex_value;
}

void *parse_Label(regmatch_t *matches, char word[])
{
    label_t *label = create_label(word);
    put_label(label);
    return label;
}

void *parse_Dec(regmatch_t *matches, char word[])
{
    int *p_dec_value = (int *)malloc(sizeof(int));
    *p_dec_value = atoi(word);
    printf("word: %s -> parse_dec: %d\n", word, *p_dec_value);
    return p_dec_value;
}

char *make_instruction_pattern()
{
    int pattern_length = 0;
    for (int i = 0; i < NUM_INSTRUCTION; i++)
    {
        pattern_length += strlen(insturction_set[i].name);
        pattern_length++;
    }

    char *pattern = (char *)malloc(pattern_length * sizeof(char));
    char *iter = pattern;
    for (int i = 0; i < NUM_INSTRUCTION; i++)
    {
        char *instr = insturction_set[i].name;
        int length = strlen(instr);
        memcpy(iter, instr, length);
        iter += length;
        *iter = '|';
        iter++;
    }
    iter--;
    *iter = '\0';
    printf("%d\n%s\n", pattern_length, pattern);
    return pattern;
}

char *make_pseudo_instruction_pattern()
{
    int pattern_length = 0;
    for (int i = 0; i < NUM_PSEUDO_INSTRUCTION; i++)
    {
        pattern_length += strlen(pseudo_instruction_set[i].name);
        pattern_length++;
    }

    char *pattern = (char *)malloc(pattern_length * sizeof(char));
    char *iter = pattern;
    for (int i = 0; i < NUM_PSEUDO_INSTRUCTION; i++)
    {
        char *instr = pseudo_instruction_set[i].name;
        int length = strlen(instr);
        memcpy(iter, instr, length);
        iter += length;
        *iter = '|';
        iter++;
    }
    iter--;
    *iter = '\0';

    return pattern;
}

void complie_parser_regex()
{
    if (PseudoInstruction_pattern == NULL)
    {
        printf("Pseudo_Instruction_pattern is NULL\nYou call make_instruction_pattern() before parsing.\n");
        exit(1);
    }

    if (Instruction_pattern == NULL)
    {
        printf("Pseudo_Instruction_pattern is NULL\nYou call make_pseudo_instruction_pattern() before parsing.\n");
        exit(1);
    }

    for (int i = 0; i < NUM_PARSER; i++)
    {
        parser_t* parser = parser_set + i;
        parser->reg = (regex_t *)malloc(sizeof(regex_t));
        printf("%d: %p %s\n", i, parser->parse,parser->pattern);
        if (regcomp(parser->reg, parser->pattern, REG_EXTENDED) != 0)
        {
            printf("pattern at %d cannot be compiled.", parser->type);
            exit(1);
        }
    }
}

void init_parser()
{
    Instruction_pattern = make_instruction_pattern();
    PseudoInstruction_pattern = make_pseudo_instruction_pattern();

    parser_set[PseudoInstruction].pattern = PseudoInstruction_pattern;
    parser_set[Instruction].pattern = Instruction_pattern;
    complie_parser_regex();
}

void refresh_matches(int num, regmatch_t matches[])
{
    for (int i = 0; i < num; i++)
    {
        matches[i].rm_so = -1;
        matches[i].rm_eo = -1;
    }
    return;
}

char *crop_match(regmatch_t matches, char word[])
{
    int length = matches.rm_eo - matches.rm_so + 1;
    char *crop = (char *)malloc(length * sizeof(char));

    memcpy(crop, word + matches.rm_so, length - 1);
    crop[length - 1] = '\0';
    return crop;
}

token_t *parse_token(char *word)
{
    if (PseudoInstruction_pattern == NULL)
    {
        printf("Pseudo_Instruction_pattern is NULL\nYou call init_parser() before parsing.\n");
        exit(1);
    }

    if (Instruction_pattern == NULL)
    {
        printf("Pseudo_Instruction_pattern is NULL\nYou call init_parser() before parsing.\n");
        exit(1);
    }

    size_t nmatch = 3;
    regmatch_t matches[3];

    for (int i = 0; i < NUM_PARSER; i++)
    {
        parser_t p = parser_set[i];
        refresh_matches(nmatch, matches);
        if (regexec(p.reg, word, nmatch, matches, 0) == REG_NOMATCH)
        {
            continue;
        }
        void *value = p.parse(matches, word);

        if (value == NULL)
        {
            printf("parser is not invalid.");
        }
        token_t *token = create_token(p.type);
        token->value = value;
        return token;
    }
    return NULL;
}

char Directive_pattern[] = "\\.[a-z]+";
char OffsetRegister_pattern[] = "([0-9]+)\\(\\$([0-9]+)\\)";
char Register_pattern[] = "\\$([0-9]+)";
char PreLabel_pattern[] = "([0-9a-zA-z]+):";
char *PseudoInstruction_pattern = NULL;
char *Instruction_pattern = NULL;
char Hex_pattern[] = "0x([0-9a-fA-F]+)";
char Label_pattern[] = "\\b[a-zA-z_][0-9a-zA-Z_]*\\b";
char Dec_pattern[] = "[0-9]+";

parser_t parser_set[9] = {
    {Directive,
     NULL,
     parse_Directive,
     Directive_pattern},
    {OffsetRegister,
     NULL,
     parse_OffsetRegister,
     OffsetRegister_pattern},
    {Register,
     NULL,
     parse_Register,
     Register_pattern},
    {PreLabel,
     NULL,
     parse_PreLabel,
     PreLabel_pattern},
    {Hex,
     NULL,
     parse_Hex,
     Hex_pattern},
    {PseudoInstruction,
     NULL,
     parse_PseudoInstruction,
     NULL},
    {Instruction,
     NULL,
     parse_Instruction,
     NULL},
    {
        Label,
        NULL,
        parse_Label,
        Label_pattern,
    },
    {Dec,
     NULL,
     parse_Dec,
     Dec_pattern}};

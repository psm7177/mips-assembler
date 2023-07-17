#ifndef _PARSER
#define _PARSER

#include <regex.h>
#include "token.h"

#define NUM_PARSER 9

typedef struct parser parser_t;
typedef void *(*parse_word)(regmatch_t *matches, char word[]);

struct parser
{
    enum TokenType type;
    regex_t* reg;
    parse_word parse;
    char *pattern;
};

extern char Directive_pattern[];
extern char Register_pattern[];
extern char OffsetRegister_pattern[];
extern char PreLabel_pattern[];
extern char *PseudoInstruction_pattern;
extern char *Instruction_pattern;
extern char Hex_pattern[];
extern char Label_pattern[];
extern char Dec_pattern[];

extern parser_t parser_set[9];

void *parse_Directive(regmatch_t *matches, char word[]);
void *parse_Register(regmatch_t *matches, char word[]);
void *parse_OffsetRegister(regmatch_t *matches, char word[]);
void *parse_PreLabel(regmatch_t *matches, char word[]);
void *parse_PseudoInstruction(regmatch_t *matches, char word[]);
void *parse_Instruction(regmatch_t *matches, char word[]);
void *parse_Hex(regmatch_t *matches, char word[]);
void *parse_Label(regmatch_t *matches, char word[]);
void *parse_Dec(regmatch_t *matches, char word[]);

token_t *parse_token(char* word);

void refresh_matches(int num, regmatch_t matches[]);
char *crop_match(regmatch_t matches, char word[]);

char *make_instruction_pattern();
char *make_pseudo_instruction_pattern();
void complie_parser_regex();
void init_parser();

#endif
#ifndef _INSTRUCTION
#define _INSTRUCTION
#include <string.h>
#define NUM_INSTRUCTION 22

;
typedef struct instruction instruction_t;
typedef struct instruction_seq instruction_seq;
typedef struct Field field_t;
typedef enum FieldType FieldType;

enum InstructionType
{
    R,
    I,
    J
};

struct instruction_seq
{
    instruction_t *arr;
    int num;
};

enum FieldType
{
    RS,
    RT,
    RD,
    SHAMT,
    FUNCT,
    IMM,
    OFFSET,
    LABEL,
    TARGET,
};

struct Field
{
    int _type;
    char offset;
    char size;
    int _default;
};

struct instruction
{
    char *name;
    enum InstructionType _type;
    char opcode;
    field_t field[5];
};

static field_t RS_FIELD;
static field_t RT_FIELD;
static field_t RD_FIELD;
static field_t SHAMT_FIELD;
static field_t FUNCT_FIELD;
static field_t IMM_FIELD;
static field_t OFFSET_FIELD;
static field_t LABEL_FIELD;
static field_t TARGET_FIELD;

static field_t field_set[9] = {
    {.offset = 6,
     .size = 5},
    {.offset = 11,
     .size = 5},
    {.offset = 16,
     .size = 5},
    {.offset = 21,
     .size = 5},
    {.offset = 26,
     .size = 6},
    {.offset = 16,
     .size = 16},
    {.offset = 16,
     .size = 16},
    {.offset = 16,
     .size = 16},
    {.offset = 6,
     .size = 26},
};
    
static instruction_t insturction_set[NUM_INSTRUCTION] =
    {
        {"addu",
         R,
         0x0,
         {
             {._type = RD, ._default = -1},
             {._type = RS, ._default = -1},
             {._type = RT, ._default = -1},
             {._type = SHAMT, ._default = 0},
             {._type = FUNCT, ._default = 0x21},
         }},
        {"and",
         R,
         0x0,
         {
             {._type = RD, ._default = -1},
             {._type = RS, ._default = -1},
             {._type = RT, ._default = -1},
             {._type = SHAMT, ._default = 0},
             {._type = FUNCT, ._default = 0x24},
         }},
        {"jr",
         R,
         0x0,
         {
             {._type = RS, ._default = -1},
             {._type = RD, ._default = 0},
             {._type = RT, ._default = 0},
             {._type = SHAMT, ._default = 0},
             {._type = FUNCT, ._default = 0x8},
         }},
        {"nor",
         R,
         0x0,
         {
             {._type = RD, ._default = -1},
             {._type = RS, ._default = -1},
             {._type = RT, ._default = -1},
             {._type = SHAMT, ._default = 0},
             {._type = FUNCT, ._default = 0x27},
         }},
        {"or",
         R,
         0x0,
         {
             {._type = RD, ._default = -1},
             {._type = RS, ._default = -1},
             {._type = RT, ._default = -1},
             {._type = SHAMT, ._default = 0},
             {._type = FUNCT, ._default = 0x25},
         }},
        {"sltu",
         R,
         0x0,
         {
             {._type = RD, ._default = -1},
             {._type = RT, ._default = -1},
             {._type = SHAMT, ._default = -1},
             {._type = RS, ._default = 0x0},
             {._type = FUNCT, ._default = 0x2b},
         }},
        {"sll",
         R,
         0x0,
         {
             {._type = RD, ._default = -1},
             {._type = RT, ._default = -1},
             {._type = SHAMT, ._default = -1},
             {._type = RS, ._default = 0x0},
             {._type = FUNCT, ._default = 0x0},
         }},
        {"srl",
         R,
         0x0,
         {
             {._type = RD, ._default = -1},
             {._type = RT, ._default = -1},
             {._type = SHAMT, ._default = -1},
             {._type = RS, ._default = 0x0},
             {._type = FUNCT, ._default = 0x2},
         }},
        {"subu",
         R,
         0x0,
         {
             {._type = RD, ._default = -1},
             {._type = RS, ._default = -1},
             {._type = RT, ._default = -1},
             {._type = SHAMT, ._default = 0},
             {._type = FUNCT, ._default = 0x23},
         }},
        {"addiu",
         I,
         0x9,
         {
             {._type = RT, ._default = -1},
             {._type = RS, ._default = -1},
             {._type = IMM, ._default = -1},
         }},
        {"andi",
         I,
         0xc,
         {
             {._type = RT, ._default = -1},
             {._type = RS, ._default = -1},
             {._type = IMM, ._default = -1},
         }},
        {"beq",
         I,
         0x4,
         {
             {._type = RS, ._default = -1},
             {._type = RT, ._default = -1},
             {._type = LABEL, ._default = -1},
         }},
        {"bne",
         I,
         0x5,
         {
             {._type = RS, ._default = -1},
             {._type = RT, ._default = -1},
             {._type = LABEL, ._default = -1},
         }},
        {"lui",
         I,
         0xf,
         {
             {._type = RT, ._default = -1},
             {._type = IMM, ._default = -1},
             {._type = RS, ._default = 0},
         }},
        {"lw",
         I,
         0x23,
         {
             {._type = RT, ._default = -1},
             {._type = OFFSET, ._default = -1},
             {._type = RS, ._default = -1},
         }},
        {"lb",
         I,
         0x20,
         {
             {._type = RT, ._default = -1},
             {._type = OFFSET, ._default = -1},
             {._type = RS, ._default = -1},
         }},
        {"ori",
         I,
         0xd,
         {
             {._type = RT, ._default = -1},
             {._type = RS, ._default = -1},
             {._type = IMM, ._default = -1},
         }},
        {"sltiu",
         I,
         0xb,
         {
             {._type = RT, ._default = -1},
             {._type = RS, ._default = -1},
             {._type = IMM, ._default = -1},
         }},
        {"sw",
         I,
         0x2b,
         {
             {._type = RT, ._default = -1},
             {._type = OFFSET, ._default = -1},
             {._type = RS, ._default = -1},
         }},
        {"sb",
         I,
         0x28,
         {
             {._type = RT, ._default = -1},
             {._type = OFFSET, ._default = -1},
             {._type = RS, ._default = -1},
         }},
        {"j",
         J,
         0x2,
         {
             {._type = TARGET, ._default = -1},
         }},
        {"jal",
         J,
         0x3,
         {
             {._type = TARGET, ._default = -1},
         }}};
#endif
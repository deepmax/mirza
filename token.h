#ifndef TOKEN_H
#define TOKEN_H

#include "types.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    TK_BAD,
    TK_FIN,
    TK_VAR,
    TK_LET,
    TK_FUNC,
    TK_TRUE,
    TK_FALSE,
    TK_IF,
    TK_ELSE,
    TK_LOOP,
    TK_FOR,
    TK_IN,
    TK_DOTDOT,
    TK_BREAK,
    TK_CONTINUE,
    TK_RETURN,
    TK_PRINT,
    TK_READ,
    TK_QUIT,
    TK_EXTERN,
    TK_LINE,
    TK_COMMA,
    TK_COLON,
    TK_SEMICOLON,
    TK_PERIOD,
    TK_BACKSLASH,
    TK_ASSIGN,
    TK_PLUS,
    TK_MINUS,
    TK_DIV,
    TK_INC,
    TK_DEC,
    TK_MUL,
    TK_MOD,
    TK_OR,
    TK_AND,
    TK_NOT,
    TK_EQ,
    TK_NE,
    TK_LT,
    TK_GT,
    TK_LTE,
    TK_GTE,
    TK_QUESTION,
    TK_L_BRACE,
    TK_R_BRACE,
    TK_L_PAREN,
    TK_R_PAREN,
    TK_L_BRACKET,
    TK_R_BRACKET,
    TK_REAL,
    TK_BOOL,
    TK_VOID,
    TK_STR,
    TK_INT,
    TK_IDENT,
    TK_LAST_TOKEN,
} token_type_t;

typedef union
{
    char* as_str;
    uint8_t as_byte;
    bool_t as_bool;
    real_t as_real;
    int32_t as_int;
    int64_t as_long;
} token_value_t;

typedef struct
{
    token_type_t type;
    token_value_t value;
    uint32_t row;
    uint32_t col;
} token_t;


#ifdef __cplusplus
}
#endif

#endif /* TOKEN_H */

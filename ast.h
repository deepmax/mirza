#ifndef AST_H
#define AST_H

#include "types.h"
#include "vector.h"
#include "token.h"
#include "context.h"
#include "jump.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef mirza_type_t(*eval_t)(void*);

struct ast_t
{
    struct ast_t* base;
    eval_t eval;
};

typedef struct ast_t ast_t;

typedef struct
{
    ast_t* base;
    mirza_type_t type;
    mirza_value_t value;
} ast_constant_t;

typedef struct
{
    ast_t* base;
    ast_t* expr;
    token_type_t op;
} ast_unary_t;

typedef struct
{
    ast_t* base;
    ast_t* lhs_expr;
    ast_t* rhs_expr;
    token_type_t op;
} ast_binary_t;

typedef struct
{
    ast_t* base;
    ast_t* expr;
} ast_print_t;

typedef struct
{
    ast_t* base;
    symbol_t* symbol;
} ast_variable_t;

typedef struct
{
    ast_t* base;
    ast_t* expr;
    symbol_t* symbol;
} ast_assign_t;

typedef struct
{
    ast_t* base;
    context_t* context;
    vector_t* nodes;
} ast_block_t;

typedef struct
{
    ast_t* base;
    ast_t* condition;
    ast_t* if_then;
    ast_t* if_else;
} ast_if_cond_t;

typedef struct
{
    ast_t* base;
    ast_t* condition;
    ast_t* body;
    loop_t* loop;

} ast_while_loop_t;

typedef struct
{
    ast_t* base;
    ast_t* init;
    ast_t* condition;
    ast_t* post;
    ast_t* body;
    loop_t* loop;

} ast_for_loop_t;

typedef struct
{
    ast_t* base;
    symbol_t* symbol;
    ast_block_t* body;
    uint16_t args;
    mirza_type_t ret_type;
} ast_func_decl_t;

typedef struct
{
    ast_t* base;
    ast_t* expr;
} ast_func_return_t;

typedef struct
{
    ast_t* base;
    symbol_t* symbol;
    vector_t* args;
} ast_func_call_t;

typedef struct
{
    ast_t* base;
    loop_t* loop;
} ast_break_loop_t;

typedef struct
{
    ast_t* base;
    loop_t* loop;
} ast_continue_loop_t;

mirza_type_t eval(ast_t* ast);
void halt();

ast_t* ast_new();
ast_constant_t* ast_new_constant(mirza_type_t type, mirza_value_t value);
ast_unary_t* ast_new_unary(token_type_t op, ast_t* expr);
ast_binary_t* ast_new_binary(token_type_t op, ast_t* lhs_expr, ast_t* rhs_expr);
ast_print_t* ast_new_print(ast_t* expr);
ast_variable_t* ast_new_variable(symbol_t* symbol);
ast_assign_t* ast_new_assign(symbol_t* symbol, ast_t* expr);
ast_block_t* ast_new_block(context_t* context);
ast_if_cond_t* ast_new_if_cond(ast_t* condition, ast_t* if_then, ast_t* if_else);
ast_while_loop_t* ast_new_while_loop(ast_t* condition, ast_t* body);
ast_for_loop_t* ast_new_for_loop(ast_t* init, ast_t* condition, ast_t* post, ast_t* body);
ast_func_decl_t* ast_new_func_decl(symbol_t* symbol, ast_block_t* body, uint16_t args, mirza_type_t ret_type);
ast_func_return_t* ast_new_func_return(ast_t* expr);
ast_func_call_t* ast_new_func_call(symbol_t* symbol, vector_t* args);
ast_break_loop_t* ast_new_break_loop(loop_t* loop);
ast_continue_loop_t* ast_new_continue_loop(loop_t* loop);

// free them all

#ifdef __cplusplus
}
#endif

#endif /* AST_H */

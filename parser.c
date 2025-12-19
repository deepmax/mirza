#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "ast.h"
#include "types.h"
#include "vm.h"
#include "context.h"
#include "panic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static token_t look;
static context_t* context;

ast_t* factor();
ast_t* expression();
ast_t* statement();
ast_t* func_call(const char* id);

typedef struct
{
    const token_type_t token_type;
    const int16_t prec;
} bin_op_prec_t;

typedef struct
{
    char* id;
    type_t type;
} param_t;

const bin_op_prec_t BIN_OP_PREC[] = {
    {TK_MUL, 90},
    {TK_DIV, 90},
    {TK_MOD, 90},
    {TK_PLUS, 80},
    {TK_MINUS, 80},
    {TK_LT, 70},
    {TK_LTE, 70},
    {TK_GT, 70},
    {TK_GTE, 70},
    {TK_EQ, 60},
    {TK_NE, 60},
    {TK_AND_BIT, 55},
    {TK_XOR_BIT, 54},
    {TK_OR_BIT, 53},
    {TK_AND, 50},
    {TK_OR, 40}
};

static const token_type_t UNARY[] = {
    TK_PLUS,
    TK_MINUS,
    TK_NOT,
};

void parser_load(const char* filename)
{
    lexer_init_file(filename);
    look.type = TK_BAD;
    look.col = 0;
    look.row = 0;
    look = lexer_next();
    context = global_context;
}

void parser_stdin()
{
    lexer_init_stdin();
    look.type = TK_BAD;
    look.col = 0;
    look.row = 0;
    look = lexer_next();
    context = global_context;
}

void parser_free()
{
    lexer_free();
}

int16_t token_prec(token_type_t token_type)
{
    for (int i = 0; i<sizeof (BIN_OP_PREC) / sizeof (BIN_OP_PREC[0]); i++)
    {
        if (BIN_OP_PREC[i].token_type == token_type)
            return BIN_OP_PREC[i].prec;
    }
    return -1;
}

bool_t is_binary(token_type_t token_type)
{
    return token_prec(token_type) > 0;
}

bool_t is_unary(token_type_t token_type)
{
    for (int i = 0; i<sizeof (UNARY) / sizeof (UNARY[0]); i++)
    {
        if (UNARY[i] == token_type)
            return true;
    }
    return false;
}

void match(token_type_t token_type)
{
    if (look.type == token_type)
        look = lexer_next();
    else
        panic("Not expected token");
}

char* peek_ident()
{
    if (look.type != TK_IDENT)
        panic("An identifier is expected");

    return look.value.as_str;
}

type_t peek_data_type()
{
    if (look.type != TK_IDENT)
        return MT_UNKNOWN;

    for (int i = 0; i < sizeof (BUILTIN_DATATYPES) / sizeof (BUILTIN_DATATYPES[0]); i++)
    {
        if (strcmp(BUILTIN_DATATYPES[i].name, look.value.as_str) == 0)
            return BUILTIN_DATATYPES[i].type;
    }

    return MT_UNKNOWN;
}

type_t data_type()
{
    match(TK_COLON);

    type_t t = peek_data_type();

    if (t != MT_UNKNOWN)
    {
        match(look.type);
        return t;
    }

    panic("Uknown data type.");

    return MT_UNKNOWN;
}

ast_t* assign(const char* id)
{
    symbol_t* s = context_get(context, id, false);

    if (s == NULL)
        panic("Identifier is not defined.");

    match(TK_ASSIGN);

    return (ast_t*) ast_new_assign(s, expression());
}

ast_t* var()
{
    match(TK_VAR);

    const char* id = peek_ident();

    match(TK_IDENT);

    if (context_get(context, id, true) != NULL)
        panic("Identifier is already defined.");

    symbol_t* s = context_add(context, id, MT_UNKNOWN);

    if (look.type == TK_COLON)
        s->type = data_type();

    if (look.type == TK_ASSIGN)
        return assign(id);

    if (s->type == MT_UNKNOWN)
        panic("No type declared for the variable.");

    return NULL;
}

ast_t* ident()
{
    const char* id = peek_ident();

    match(TK_IDENT);

    if (look.type == TK_ASSIGN)
        return assign(id);

    if (context_get(context, id, false) == NULL)
        panic("Identifier is not defined.");

    if (look.type == TK_L_PAREN)
        return func_call(id);

    return (ast_t*) ast_new_variable(context_get(context, id, false));
}

ast_t* binary_expr(int16_t min_prec, ast_t* lhs)
{
    while (true)
    {
        int32_t tok_prec = token_prec(look.type);
        if (tok_prec < min_prec)
            break;

        token_type_t op = look.type;

        match(op);

        ast_t* rhs = factor();

        int16_t next_prec = token_prec(look.type);

        if (tok_prec < next_prec)
            rhs = binary_expr(tok_prec + 1, rhs);

        lhs = (ast_t*) ast_new_binary(op, lhs, rhs);
    }

    return lhs;
}

ast_t* unary_expr()
{
    token_type_t unary_token = look.type;
    match(unary_token);
    return (ast_t*) ast_new_unary(unary_token, factor());
}

ast_t* factor()
{
    ast_t* node = NULL;

    if (look.type == TK_L_PAREN)
    {
        match(TK_L_PAREN);
        node = expression();
        match(TK_R_PAREN);
    }
    else if (look.type == TK_IDENT)
    {
        node = ident();
    }
    else if (look.type == TK_TRUE)
    {
        match(TK_TRUE);
        value_t value;
        value.as_int64 = 1;
        node = (ast_t*) ast_new_constant(MT_INT64, value);
    }
    else if (look.type == TK_FALSE)
    {
        match(TK_FALSE);
        value_t value;
        value.as_int64 = 0;
        node = (ast_t*) ast_new_constant(MT_INT64, value);
    }
    else if (look.type == TK_INT64)
    {
        value_t value;
        value.as_int64 = look.value.as_int;
        match(TK_INT64);
        node = (ast_t*) ast_new_constant(MT_INT64, value);
    }
    else if (look.type == TK_REAL)
    {
        value_t value;
        value.as_real = look.value.as_real;
        match(TK_REAL);
        node = (ast_t*) ast_new_constant(MT_REAL, value);
    }
    else if (look.type == TK_STR)
    {
        value_t value;
        value.as_str = look.value.as_str;
        match(TK_STR);
        node = (ast_t*) ast_new_constant(MT_STR, value);
    }
    else if (is_unary(look.type))
    {
        node = unary_expr();
    }
    else
    {
        panic("Unknown factor!");
    }

    return node;
}

ast_t* expression()
{
    return binary_expr(0, factor());
}

ast_t* semicolon()
{
    match(TK_SEMICOLON);
    return NULL;
}

ast_t* print()
{
    match(TK_PRINT);
    return (ast_t*) ast_new_print(expression());
}

ast_t* block(block_t type, vector_t* params)
{
    match(TK_L_BRACE);

    context_t* new_context = context_new(context, type);
    
    if (params != NULL)
    {
        for (size_t i = 0; i < vec_size(params); i++)
        {
            param_t* param = vec_get(params, i);
            context_add(new_context, param->id, param->type);
        }
    }

    ast_block_t* blck = ast_new_block(new_context);

    context = new_context;

    vec_append(blck->nodes, NULL);

    while (look.type != TK_R_BRACE)
    {
        vec_append(blck->nodes, statement());
    }

    match(TK_R_BRACE);

    context = new_context->parent;

    return (ast_t*) blck;
}

ast_t* func_decl()
{
    match(TK_FUNC);

    const char* id = peek_ident();
    match(TK_IDENT);

    if (context_get(context, id, false) != NULL)
        panic("Identifier is already defined.");

    symbol_t* s = context_add(context, id, MT_FUNC);

    match(TK_L_PAREN);

    vector_t* params = vec_new(0);

    while (look.type != TK_R_PAREN)
    {
        param_t* param = malloc(sizeof(param_t));
        param->id = peek_ident();
        match(TK_IDENT);
        
        param->type = data_type();

        vec_append(params, param);

        if (look.type == TK_R_PAREN)
            break;
        match(TK_COMMA);
    }
    match(TK_R_PAREN);

    type_t ret_type = data_type();

    s->type = MT_FUNC;

    return (ast_t*) ast_new_func_decl(s, (ast_block_t*) block(MB_FUNC, params), vec_size(params), ret_type);
}

ast_t* func_ret()
{
    if (context_get_func(context) == NULL)
        panic("Return statement outside of function.");
    match(TK_RETURN);
    // according to func return type, expect an expression or nothing
    return (ast_t*) ast_new_func_return(expression());
}

ast_t* func_call(const char* id)
{
    symbol_t* s = context_get(context, id, false);

    if (s == NULL)
        panic("Identifier is not defined.");

    match(TK_L_PAREN);

    vector_t* args = vec_new(0);

    while (look.type != TK_R_PAREN)
    {
        vec_append(args, expression());
        if (look.type == TK_R_PAREN)
            break;
        match(TK_COMMA);
    }
    match(TK_R_PAREN);

    return (ast_t*) ast_new_func_call(s, args);
}

ast_t* if_cond()
{
    match(TK_IF);
    ast_t* condition = expression();
    ast_t* if_then = block(MB_NORMAL, NULL);
    ast_t* if_else = NULL;
    if (look.type == TK_ELSE)
    {
        match(TK_ELSE);
        if (look.type == TK_IF)
            if_else = if_cond();
        else
            if_else = block(MB_NORMAL, NULL);
    }
    return (ast_t*) ast_new_if_cond(condition, if_then, if_else);
}

ast_t* for_loop()
{
    context_t* new_context = context_new(context, MB_NORMAL);
    context = new_context;

    match(TK_FOR);
    ast_t* init = look.type == TK_VAR? var() : expression();
    match(TK_SEMICOLON);
    ast_t* condition = expression();
    match(TK_SEMICOLON);
    ast_t* post = expression();

    ast_block_t* for_block = (ast_block_t*) ast_new_for_loop(init, condition, post, block(MB_LOOP, NULL));

    context = new_context->parent;

    return (ast_t*) for_block;
}

ast_t* break_loop()
{
    if (context_get_loop(context) == NULL)
        panic("Break statement outside of loop.");
    match(TK_BREAK);
    return (ast_t*) ast_new_break_loop(context_get_loop(context));
}

ast_t* continue_loop()
{
    if (context_get_loop(context) == NULL)
        panic("Continue statement outside of loop.");
    match(TK_CONTINUE);
    return (ast_t*) ast_new_continue_loop(context_get_loop(context));
}

ast_t* statement()
{
    switch (look.type)
    {
    case TK_SEMICOLON:
        return semicolon();
    case TK_VAR:
        return var();
    case TK_PRINT:
        return print();
    case TK_IF:
        return if_cond();
    case TK_L_BRACE:
        return block(MB_NORMAL, NULL);
    case TK_FOR:
        return for_loop();
    case TK_FUNC:
        return func_decl();
    case TK_BREAK:
        return break_loop();
    case TK_CONTINUE:
        return continue_loop();
    case TK_RETURN:
        return func_ret();
    default:
        return expression();
    }
}

void parser_start(bool_t execute, bool_t dasm)
{
    vm_init(2048, 512);

    ast_block_t* block = ast_new_block(global_context);

    while (look.type != TK_FIN)
        vec_append(block->nodes, statement());

    eval((ast_t*) block);

    if (dasm)
    {
        vm_dasm();
    }
    
    if (execute)
    {
        vm_exec();
    }
}

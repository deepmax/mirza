#include "ast.h"
#include "vm.h"
#include "jump.h"
#include "panic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

mirza_type_t eval(ast_t* ast)
{
    return ast ? ast->base->eval(ast) : MT_UNKNOWN;
}

void halt()
{
    EMIT(HALT);
}

mirza_type_t eval_constant(ast_constant_t* ast)
{
    if (ast->type == MT_INT)
    {
        if (ast->value.as_int == 0)
            EMIT(ICONST_0);
        else if (ast->value.as_int == 1)
            EMIT(ICONST_1);
        else
            EMIT(ICONST, NUM32(ast->value.as_int));
    }
    else if (ast->type == MT_REAL)
    {
        EMIT(RCONST, NUM64(ast->value.as_ulong));
    }
    else if (ast->type == MT_STR)
    {
        uint16_t a = vm_data_addr();
        vm_data_emit((byte_t*)ast->value.as_str, strlen(ast->value.as_str) + 1);
        EMIT(ACONST, NUM16(a));
    }
    return ast->type;
}

mirza_type_t eval_unary(ast_unary_t* ast)
{
    mirza_type_t out = eval(ast->expr);

    if (out == MT_INT)
    {
        switch (ast->op)
        {
        case TK_PLUS:
            break;
        case TK_MINUS:
            EMIT(INEG);
            break;
        case TK_NOT:
            EMIT(INOT);
            break;
        default:
        }
        return out;
    }
    else if (out == MT_REAL)
    {
        switch (ast->op)
        {
        case TK_PLUS:
            break;
        case TK_MINUS:
            EMIT(RNEG);
            break;
        default:
        }
        return out;
    }

    panic("Unary error");

    return MT_UNKNOWN;
}

mirza_type_t eval_binary(ast_binary_t* ast)
{
    mirza_type_t l_out = eval(ast->lhs_expr);
    mirza_type_t r_out = eval(ast->rhs_expr);

    if (l_out == MT_INT && r_out == MT_INT)
    {
        switch (ast->op)
        {
        case TK_PLUS:
            EMIT(IADD);
            break;
        case TK_MINUS:
            EMIT(ISUB);
            break;
        case TK_MUL:
            EMIT(IMUL);
            break;
        case TK_DIV:
            EMIT(IDIV);
            break;
        case TK_MOD:
            EMIT(IMOD);
            break;
        case TK_EQ:
            EMIT(IEQ);
            break;
        case TK_LT:
            EMIT(ILT);
            break;
        case TK_LTE:
            EMIT(ILE);
            break;
        case TK_GT:
            EMIT(IGT);
            break;
        case TK_GTE:
            EMIT(IGE);
            break;
        case TK_AND:
            EMIT(IAND);
            break;
        case TK_OR:
            EMIT(IOR);
            break;
        default:
        }
        return MT_INT;
    }
    else if (l_out == MT_REAL && r_out == MT_REAL)
    {
        switch (ast->op)
        {
        case TK_PLUS:
            EMIT(RADD);
            break;
        case TK_MINUS:
            EMIT(RSUB);
            break;
        case TK_MUL:
            EMIT(RMUL);
            break;
        case TK_DIV:
            EMIT(RDIV);
            break;
        case TK_MOD:
            EMIT(RMOD);
            break;
        case TK_EQ:
            EMIT(REQ);
            break;
        case TK_LT:
            EMIT(RLT);
            break;
        case TK_GT:
            EMIT(RGT);
            break;
        default:
        }
        return MT_REAL;
    }

    // other types: float, string, ..

    panic("Binary error");

    return MT_UNKNOWN;
}

mirza_type_t eval_print(ast_print_t* ast)
{
    mirza_type_t out = eval(ast->expr);

    switch (out)
    {
    case MT_INT:
        EMIT(IPRINT);
        break;
    case MT_REAL:
        EMIT(RPRINT);
        break;
    case MT_STR:
        EMIT(APRINT);
        break;
    default:
    }

    return MT_UNKNOWN;
}

mirza_type_t eval_variable(ast_variable_t* ast)
{
    if (ast->symbol->type == MT_INT)
    {
        if (ast->symbol->global)
            EMIT(ILOADG, NUM16(ast->symbol->addr));
        else
            EMIT(ILOAD, NUM16(ast->symbol->addr));
    }
    else if (ast->symbol->type == MT_REAL)
    {
        if (ast->symbol->global)
            EMIT(RLOADG, NUM16(ast->symbol->addr));
        else
            EMIT(RLOAD, NUM16(ast->symbol->addr));
    }
    else if (ast->symbol->type == MT_STR)
    {
        EMIT(ALOAD, NUM16(ast->symbol->addr));
    }
    return ast->symbol->type;
}

mirza_type_t eval_assign(ast_assign_t* ast)
{
    mirza_type_t out = eval(ast->expr);

    if (ast->symbol->type != MT_UNKNOWN && ast->symbol->type != out)
        panic("Assignment type mismatch");

    if (out == MT_INT)
    {
        if (ast->symbol->global)
            EMIT(ISTOREG, NUM16(ast->symbol->addr));
        else
            EMIT(ISTORE, NUM16(ast->symbol->addr));
    }
    else if (out == MT_REAL)
    {
        if (ast->symbol->global)
            EMIT(RSTOREG, NUM16(ast->symbol->addr));
        else
            EMIT(RSTORE, NUM16(ast->symbol->addr));
    }
    else if (out == MT_STR)
    {
        EMIT(ASTORE, NUM16(ast->symbol->addr));
    }

    ast->symbol->type = out;

    return out;
}

mirza_type_t eval_block(ast_block_t* ast)
{
    if (context_is_global(ast->context))
    {
        uint16_t vars = context_allocated(ast->context);
        uint16_t args = 0;
        EMIT(ICONST_0, ICONST_0, PROC, NUM16(args), NUM16(vars - args));
    }

    for (size_t i = 0; i < vec_size(ast->nodes); i++)
        eval(vec_get(ast->nodes, i));

    if (context_is_global(ast->context))
        halt();

    return MT_UNKNOWN;
}

mirza_type_t eval_if_cond(ast_if_cond_t* ast)
{
    jump_t* else_addr = jump_new();
    jump_t* exit_addr = jump_new();

    eval(ast->condition);

    EMIT(JEZ);

    jump_to(else_addr);

    eval(ast->if_then);

    EMIT(JMP);

    jump_to(exit_addr);

    jump_label(else_addr);

    if (ast->if_else != NULL)
        eval(ast->if_else);

    jump_label(exit_addr);

    jump_fix(else_addr);
    jump_fix(exit_addr);

    return MT_UNKNOWN;
}

mirza_type_t eval_while_loop(ast_while_loop_t* ast)
{
    if (ast->loop == NULL)
        return MT_UNKNOWN;
    
    jump_label(ast->loop->begin);

    eval(ast->condition);

    EMIT(JEZ);

    jump_to(ast->loop->end);

    eval(ast->body);

    EMIT(JMP);

    jump_to(ast->loop->begin);

    jump_label(ast->loop->end);

    jump_fix(ast->loop->begin);
    jump_fix(ast->loop->end);

    return MT_UNKNOWN;
}

mirza_type_t eval_for_loop(ast_for_loop_t* ast)
{
    if (ast->loop == NULL)
        return MT_UNKNOWN;

    eval(ast->init);

    jump_label(ast->loop->begin);

    eval(ast->condition);

    EMIT(JEZ);

    jump_to(ast->loop->end);

    eval(ast->body);

    eval(ast->post);

    EMIT(JMP);

    jump_to(ast->loop->begin);

    jump_label(ast->loop->end);

    jump_fix(ast->loop->begin);
    jump_fix(ast->loop->end);

    return MT_UNKNOWN;
}

mirza_type_t eval_func_decl(ast_func_decl_t* ast)
{
    jump_t* func_end = jump_new();
    jump_t* func_beg = jump_new();

    EMIT(JMP);
    jump_to(func_end);
    jump_label(func_beg);

    uint16_t vars = context_allocated(ast->body->context);
    uint16_t args = ast->args;
    EMIT(PROC, NUM16(args), NUM16(vars - args));

    ast->symbol->addr = func_beg->label;

    eval((ast_t*) ast->body);

    EMIT(ICONST_0, RET);

    jump_label(func_end);

    jump_fix(func_end);
    jump_fix(func_beg);

    return MT_UNKNOWN;
}

mirza_type_t eval_func_return(ast_func_return_t* ast)
{
    mirza_type_t out = eval(ast->expr);
    EMIT(RET);
    return out;
}

mirza_type_t eval_func_call(ast_func_call_t* ast)
{
    for (size_t i = 0; i < vec_size(ast->args); i++)
        eval(vec_get(ast->args, i));
    EMIT(CALL, NUM16(ast->symbol->addr));
    return MT_INT;
}

mirza_type_t eval_break_loop(ast_break_loop_t* ast)
{
    EMIT(JMP);

    jump_to(ast->loop->end);

    return MT_UNKNOWN;
}

mirza_type_t eval_continue_loop(ast_continue_loop_t* ast)
{
    EMIT(JMP);

    jump_to(ast->loop->begin);

    return MT_UNKNOWN;
}

ast_t* ast_new()
{
    ast_t* ast = malloc(sizeof (ast_t));
    ast->base = NULL;
    ast->eval = NULL;
    return ast;
}

ast_constant_t* ast_new_constant(mirza_type_t type, mirza_value_t value)
{
    ast_constant_t* ast_constant = malloc(sizeof (ast_constant_t));
    ast_constant->base = ast_new();
    ast_constant->base->eval = (eval_t) eval_constant;
    ast_constant->type = type;
    ast_constant->value = value;
    return ast_constant;
}

ast_unary_t* ast_new_unary(token_type_t op, ast_t* expr)
{
    ast_unary_t* ast_unary = malloc(sizeof (ast_unary_t));
    ast_unary->base = ast_new();
    ast_unary->base->eval = (eval_t) eval_unary;
    ast_unary->op = op;
    ast_unary->expr = expr;
    return ast_unary;
}

ast_binary_t* ast_new_binary(token_type_t op, ast_t* lhs_expr, ast_t* rhs_expr)
{
    ast_binary_t* ast_binary = malloc(sizeof (ast_binary_t));
    ast_binary->base = ast_new();
    ast_binary->base->eval = (eval_t) eval_binary;
    ast_binary->op = op;
    ast_binary->lhs_expr = lhs_expr;
    ast_binary->rhs_expr = rhs_expr;
    return ast_binary;
}

ast_print_t* ast_new_print(ast_t* expr)
{
    ast_print_t* ast_print = malloc(sizeof (ast_print_t));
    ast_print->base = ast_new();
    ast_print->base->eval = (eval_t) eval_print;
    ast_print->expr = expr;
    return ast_print;
}

ast_variable_t* ast_new_variable(symbol_t* symbol)
{
    ast_variable_t* ast_variable = malloc(sizeof (ast_variable_t));
    ast_variable->base = ast_new();
    ast_variable->base->eval = (eval_t) eval_variable;
    ast_variable->symbol = symbol;
    return ast_variable;
}

ast_assign_t* ast_new_assign(symbol_t* symbol, ast_t* expr)
{
    ast_assign_t* ast_assign = malloc(sizeof (ast_assign_t));
    ast_assign->base = ast_new();
    ast_assign->base->eval = (eval_t) eval_assign;
    ast_assign->symbol = symbol;
    ast_assign->expr = expr;
    return ast_assign;
}

ast_block_t* ast_new_block(context_t* context)
{
    ast_block_t* ast_block = malloc(sizeof (ast_block_t));
    ast_block->base = ast_new();
    ast_block->base->eval = (eval_t) eval_block;
    ast_block->context = context;
    ast_block->nodes = vec_new(0);
    return ast_block;
}

ast_if_cond_t* ast_new_if_cond(ast_t* condition, ast_t* if_then, ast_t* if_else)
{
    ast_if_cond_t* ast_if_cond = malloc(sizeof (ast_if_cond_t));
    ast_if_cond->base = ast_new();
    ast_if_cond->base->eval = (eval_t) eval_if_cond;
    ast_if_cond->condition = condition;
    ast_if_cond->if_then = if_then;
    ast_if_cond->if_else = if_else;
    return ast_if_cond;
}

ast_while_loop_t* ast_new_while_loop(ast_t* condition, ast_t* body)
{
    ast_while_loop_t* ast_while_loop = malloc(sizeof (ast_while_loop_t));
    ast_while_loop->base = ast_new();
    ast_while_loop->base->eval = (eval_t) eval_while_loop;
    ast_while_loop->condition = condition;
    ast_while_loop->body = body;
    ast_while_loop->loop = body == NULL ? NULL : context_get_loop(((ast_block_t*) body)->context);
    return ast_while_loop;
}

ast_for_loop_t* ast_new_for_loop(ast_t* init, ast_t* condition, ast_t* post, ast_t* body)
{
    ast_for_loop_t* ast_for_loop = malloc(sizeof (ast_for_loop_t));
    ast_for_loop->base = ast_new();
    ast_for_loop->base->eval = (eval_t) eval_for_loop;
    ast_for_loop->init = init;
    ast_for_loop->condition = condition;
    ast_for_loop->post = post;
    ast_for_loop->body = body;
    ast_for_loop->loop = body == NULL ? NULL : context_get_loop(((ast_block_t*) body)->context);
    return ast_for_loop;
}

ast_func_decl_t* ast_new_func_decl(symbol_t* symbol, ast_block_t* body, uint16_t args, mirza_type_t ret_type)
{
    ast_func_decl_t* ast_func_decl = malloc(sizeof (ast_func_decl_t));
    ast_func_decl->base = ast_new();
    ast_func_decl->base->eval = (eval_t) eval_func_decl;
    ast_func_decl->symbol = symbol;
    ast_func_decl->body = body;
    ast_func_decl->args = args;
    ast_func_decl->ret_type = ret_type;
    return ast_func_decl;
}

ast_func_return_t* ast_new_func_return(ast_t* expr)
{
    ast_func_return_t* ast_func_return = malloc(sizeof (ast_func_return_t));
    ast_func_return->base = ast_new();
    ast_func_return->base->eval = (eval_t) eval_func_return;
    ast_func_return->expr = expr;
    return ast_func_return;
}

ast_func_call_t* ast_new_func_call(symbol_t* symbol, vector_t* args)
{
    ast_func_call_t* ast_func_call = malloc(sizeof (ast_func_call_t));
    ast_func_call->base = ast_new();
    ast_func_call->base->eval = (eval_t) eval_func_call;
    ast_func_call->symbol = symbol;
    ast_func_call->args = args;
    return ast_func_call;
}

ast_break_loop_t* ast_new_break_loop(loop_t* loop)
{
    ast_break_loop_t* ast_break_loop = malloc(sizeof (ast_break_loop_t));
    ast_break_loop->base = ast_new();
    ast_break_loop->base->eval = (eval_t) eval_break_loop;
    ast_break_loop->loop = loop;
    return ast_break_loop;
}

ast_continue_loop_t* ast_new_continue_loop(loop_t* loop)
{
    ast_continue_loop_t* ast_continue_loop = malloc(sizeof (ast_continue_loop_t));
    ast_continue_loop->base = ast_new();
    ast_continue_loop->base->eval = (eval_t) eval_continue_loop;
    ast_continue_loop->loop = loop;
    return ast_continue_loop;
}

#include "ast.h"
#include "vm.h"
#include "jump.h"
#include "panic.h"
#include "utf8.h"
#include "builtin.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stddef.h>

type_t eval(ast_t* ast)
{
    return ast ? ast->base->eval(ast) : MT_UNKNOWN;
}

void halt()
{
    EMIT(HALT);
}

// Helper function to check if a type is an integer type
// Java strategy: only signed integers are supported
static bool is_integer_type(type_t type)
{
    return type == MT_INT8 || type == MT_INT16 || type == MT_INT32 || type == MT_INT64;
}

// Helper function to emit constant opcode based on type
static void emit_integer_constant(type_t type, value_t value)
{
    // Use optimized constants for 0 and 1
    if (type == MT_INT8 || type == MT_INT16 || type == MT_INT32 || type == MT_INT64) {
        int64_t val;
        switch (type) {
            case MT_INT8:  val = (int64_t)value.as_int8; break;
            case MT_INT16: val = (int64_t)value.as_int16; break;
            case MT_INT32: val = (int64_t)value.as_int32; break;
            case MT_INT64: val = value.as_int64; break;
            default: return;
        }
        if (val == 0) {
            EMIT(ICONST_0);
        } else if (val == 1) {
            EMIT(ICONST_1);
        } else {
            // Use type-specific opcodes for shorter bytecode
            switch (type) {
                case MT_INT8:
                    EMIT(I8CONST, NUM8((int8_t)val));
                    break;
                case MT_INT16:
                    EMIT(I16CONST, NUM16((int16_t)val));
                    break;
                case MT_INT32:
                    EMIT(I32CONST, NUM32((int32_t)val));
                    break;
                case MT_INT64:
                    EMIT(ICONST, NUM64(val));
                    break;
                default:
                    EMIT(ICONST, NUM64(val));
                    break;
            }
        }
    }
}

// Helper function to emit conversion opcode
// Java strategy: emit conversions for all smaller types (i8cast, i16cast, i32cast)
static void emit_conversion(type_t from, type_t to)
{
    if (from == to) return;
    // All operations work on int64, so we need to truncate when converting to smaller types
    switch (to) {
        case MT_INT8:  EMIT(I8CAST); break;  // int64 to int8
        case MT_INT16: EMIT(I16CAST); break; // int64 to int16
        case MT_INT32: EMIT(I32CAST); break; // int64 to int32
        case MT_INT64: // No-op, already int64
        default: break;
    }
}


type_t eval_constant(ast_constant_t* ast)
{
    if (is_integer_type(ast->type))
    {
        emit_integer_constant(ast->type, ast->value);
        // Convert to int64 for unified operations
        emit_conversion(ast->type, MT_INT64);
        return MT_INT64; // Operations normalize to int64
    }
    else if (ast->type == MT_REAL)
    {
        if (ast->value.as_real == 0.0)
        {
            EMIT(RCONST_0);
        }
        else if (ast->value.as_real == 1.0)
        {
            EMIT(RCONST_1);
        }
        else
        {
            EMIT(RCONST, NUM64(ast->value.as_uint64));
        }
    }
    else if (ast->type == MT_STR)
    {
        uint16_t a = vm_data_addr();
        vm_data_emit((uint8_t*)ast->value.as_str, utf8size((utf8_int8_t*)ast->value.as_str));
        EMIT(SCONST, NUM16(a));
    }
    return ast->type;
}

type_t eval_unary(ast_unary_t* ast)
{
    type_t out = eval(ast->expr);

    if (is_integer_type(out))
    {
        // Convert to int64 for operations if not already
        if (out != MT_INT64) {
            emit_conversion(out, MT_INT64);
        }
        // Operations work on int64
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
            ;
        }
        return MT_INT64; // Operations normalize to int64
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
            ;
        }
        return out;
    }

    panic("Unary error");

    return MT_UNKNOWN;
}

type_t eval_binary(ast_binary_t* ast)
{
    type_t l_out = eval(ast->lhs_expr);
    type_t r_out = eval(ast->rhs_expr);

    if (is_integer_type(l_out) && is_integer_type(r_out))
    {
        // Convert both operands to int64 for operations if not already
        if (l_out != MT_INT64) {
            emit_conversion(l_out, MT_INT64);
        }
        if (r_out != MT_INT64) {
            emit_conversion(r_out, MT_INT64);
        }
        // Operations work on int64
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
        case TK_NE:
            EMIT(INQ);
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
        case TK_AND_BIT:
            EMIT(IBAND);
            break;
        case TK_OR_BIT:
            EMIT(IBOR);
            break;
        case TK_XOR_BIT:
            EMIT(IBXOR);
            break;
        case TK_AND:
            EMIT(IAND);
            break;
        case TK_OR:
            EMIT(IOR);
            break;
        // Note: Shift operators (ISHL, ISHR) exist in VM but no token types yet
        default:
            ;
        }
        return MT_INT64; // Operations normalize to int64
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
        case TK_NE:
            EMIT(RNQ);
            break;
        case TK_LT:
            EMIT(RLT);
            break;
        case TK_GT:
            EMIT(RGT);
            break;
        default:
            ;
        }
        return MT_REAL;
    }

    // other types: float, string, ..

    panic("Binary error");

    return MT_UNKNOWN;
}

type_t eval_print(ast_print_t* ast)
{
    type_t out = eval(ast->expr);

    if (is_integer_type(out))
    {
        // All integer types are represented as int64 on the stack
        // No conversion needed for printing (value is already in int64 format)
        EMIT(IPRINT);
    }
    else if (out == MT_REAL)
    {
        EMIT(RPRINT);
    }
    else if (out == MT_STR)
    {
        EMIT(SPRINT);
    }
    else
    {
        panic("Print error. Unknown type.");
        return MT_UNKNOWN;
    }
    
    return out;
}

type_t eval_variable(ast_variable_t* ast)
{
    type_t var_type = ast->symbol->type;
    bool is_global = ast->symbol->global;
    uint16_t addr = ast->symbol->addr;
    
    if (is_integer_type(var_type)) {
        EMIT(ILOAD, var_type, NUM16(addr), is_global ? 1 : 0);
        // Return original type to preserve signed/unsigned information
        // Operations will convert to int64 if needed
        return var_type;
    } else if (var_type == MT_REAL) {
        if (is_global) EMIT(RLOAD, NUM16(addr));
        else EMIT(RLOAD, NUM16(addr));
    } else if (var_type == MT_STR) {
        EMIT(SLOAD, NUM16(addr));
    }
    return var_type;
}

type_t eval_assign(ast_assign_t* ast)
{
    type_t expr_type = eval(ast->expr);
    type_t var_type = ast->symbol->type;
    bool is_global = ast->symbol->global;
    uint16_t addr = ast->symbol->addr;

    // If variable type is unknown, infer from expression
    if (var_type == MT_UNKNOWN) {
        var_type = expr_type;
        ast->symbol->type = var_type;
    }

    // Convert expression result to variable type if needed
    if (is_integer_type(expr_type) && is_integer_type(var_type)) {
        // Expression is already normalized to int64, convert to variable type
        emit_conversion(MT_INT64, var_type);
    } else if (expr_type != var_type && !(is_integer_type(expr_type) && is_integer_type(var_type))) {
        panic("Assignment type mismatch");
    }

    // Store using unified opcode
    if (is_integer_type(var_type)) {
        EMIT(ISTORE, var_type, NUM16(addr), is_global ? 1 : 0);
    } else if (var_type == MT_REAL) {
        if (is_global) EMIT(RSTORE, NUM16(addr));
        else EMIT(RSTORE, NUM16(addr));
    } else if (var_type == MT_STR) {
        EMIT(SSTORE, NUM16(addr));
    }

    return var_type;
}

type_t eval_block(ast_block_t* ast)
{
    if (context_is_global(ast->context))
    {
        uint16_t vars = context_allocated(ast->context);
        uint16_t args = 0;
        EMIT(ICONST_0, ICONST_0, PROC, NUM16(args), NUM16((vars - args)));
    }

    for (size_t i = 0; i < vec_size(ast->nodes); i++)
        eval(vec_get(ast->nodes, i));

    if (context_is_global(ast->context))
        halt();

    return MT_UNKNOWN;
}

type_t eval_if_cond(ast_if_cond_t* ast)
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

type_t eval_for_loop(ast_for_loop_t* ast)
{
    if (ast->loop == NULL)
        return MT_UNKNOWN;

    eval(ast->init);

    jump_label(ast->loop->begin);

    eval(ast->condition);

    EMIT(JEZ);

    jump_to(ast->loop->end);

    eval(ast->body);

    jump_label(ast->loop->post);

    eval(ast->post);

    EMIT(JMP);

    jump_to(ast->loop->begin);

    jump_label(ast->loop->end);

    jump_fix(ast->loop->begin);
    jump_fix(ast->loop->end);
    jump_fix(ast->loop->post);

    return MT_UNKNOWN;
}

type_t eval_func_decl(ast_func_decl_t* ast)
{
    jump_t* func_end = jump_new();
    jump_t* func_beg = jump_new();

    EMIT(JMP);
    jump_to(func_end);
    jump_label(func_beg);

    uint16_t vars = context_allocated(ast->body->context);
    uint16_t args = ast->args;
    EMIT(PROC, NUM16(args), NUM16((vars - args)));

    ast->symbol->addr = func_beg->label;

    eval((ast_t*) ast->body);

    EMIT(ICONST_0, RET);

    jump_label(func_end);

    jump_fix(func_end);
    jump_fix(func_beg);

    return MT_UNKNOWN;
}

type_t eval_func_return(ast_func_return_t* ast)
{
    type_t out = eval(ast->expr);
    // Note: Return type conversion should be handled at function call site
    // For now, we return the expression type
    EMIT(RET);
    return out;
}

// Helper function to check if a type is in the acceptable types array
static bool is_type_acceptable(type_t type, const type_t* acceptable_types)
{
    if (acceptable_types == NULL)
        return true;  // No type restrictions
    
    for (size_t i = 0; acceptable_types[i] != MT_UNKNOWN; i++)
    {
        if (acceptable_types[i] == type)
            return true;
    }
    return false;
}

type_t eval_func_call(ast_func_call_t* ast)
{
    // Check if this is a builtin function (marked by special addr value)
    if (ast->symbol->addr == 0xFFFF)
    {
        // Look up the builtin function by name
        const builtin_func_t* builtin = builtin_lookup(ast->symbol->id);
        if (builtin == NULL)
        {
            panic("Builtin function not found.");
        }
        
        // Evaluate arguments and check types
        for (size_t i = 0; i < vec_size(ast->args); i++)
        {
            type_t arg_type = eval(vec_get(ast->args, i));
            
            // Check if the argument type is acceptable
            if (!is_type_acceptable(arg_type, builtin->acceptable_types))
            {
                panic("Builtin function argument type mismatch.");
            }
        }
        
        // Emit the builtin function opcode
        EMIT(builtin->opcode);
        
        return builtin->ret_type;
    }
    
    // Regular user function call
    for (size_t i = 0; i < vec_size(ast->args); i++)
        eval(vec_get(ast->args, i));
    EMIT(CALL, NUM16(ast->symbol->addr));
    
    // Get function's return type and convert if needed
    type_t ret_type = ast->symbol->ret_type;
    if (ret_type == MT_UNKNOWN || ret_type == MT_VOID) {
        ret_type = MT_INT64;  // Default to int64 if not set
    }
    
    // Function returns int64 on stack, convert to declared return type
    if (is_integer_type(ret_type)) {
        emit_conversion(MT_INT64, ret_type);
    }
    
    return ret_type;
}

type_t eval_break_loop(ast_break_loop_t* ast)
{
    EMIT(JMP);

    jump_to(ast->loop->end);

    return MT_UNKNOWN;
}

type_t eval_continue_loop(ast_continue_loop_t* ast)
{
    EMIT(JMP);

    jump_to(ast->loop->post);

    return MT_UNKNOWN;
}

ast_t* ast_new()
{
    ast_t* ast = malloc(sizeof (ast_t));
    ast->base = NULL;
    ast->eval = NULL;
    return ast;
}

ast_constant_t* ast_new_constant(type_t type, value_t value)
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

ast_func_decl_t* ast_new_func_decl(symbol_t* symbol, ast_block_t* body, uint16_t args, type_t ret_type)
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

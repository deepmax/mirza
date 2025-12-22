#include "builtin.h"
#include "types.h"
#include "vm.h"
#include <string.h>

// Acceptable type arrays for builtin functions
static const type_t INTEGER_TYPES[] = {MT_INT8, MT_INT16, MT_INT32, MT_INT64, MT_UNKNOWN};
static const type_t REAL_TYPES[] = {MT_REAL, MT_UNKNOWN};
static const type_t STR_TYPES[] = {MT_STR, MT_UNKNOWN};
static const type_t NUMERIC_TYPES[] = {MT_INT8, MT_INT16, MT_INT32, MT_INT64, MT_REAL, MT_UNKNOWN};
static const type_t PRINT_TYPES[] = {MT_INT8, MT_INT16, MT_INT32, MT_INT64, MT_REAL, MT_STR, MT_UNKNOWN};

// Builtin function registry
static const builtin_func_t BUILTIN_FUNCTIONS[] = {
    {"print", 255, MT_VOID, 0, true, PRINT_TYPES},  // arg_count 255 means variadic, opcode 0 means dispatch based on type
    {"abs", 1, MT_UNKNOWN, 0, true, NUMERIC_TYPES},  // opcode 0 means dispatch based on type
    {"mod", 2, MT_REAL, RMOD, true, REAL_TYPES},
    {"pow", 2, MT_REAL, RPOW, true, REAL_TYPES},
    {"sqrt", 1, MT_REAL, RSQRT, true, REAL_TYPES},
    {"exp", 1, MT_REAL, REXP, true, REAL_TYPES},
    {"sin", 1, MT_REAL, RSIN, true, REAL_TYPES},
    {"cos", 1, MT_REAL, RCOS, true, REAL_TYPES},
    {"tan", 1, MT_REAL, RTAN, true, REAL_TYPES},
    {"acos", 1, MT_REAL, RACOS, true, REAL_TYPES},
    {"atan2", 2, MT_REAL, RATAN2, true, REAL_TYPES},
    {"log", 1, MT_REAL, RLOG, true, REAL_TYPES},
    {"log10", 1, MT_REAL, RLOG10, true, REAL_TYPES},
    {"log2", 1, MT_REAL, RLOG2, true, REAL_TYPES},
    {"ceil", 1, MT_REAL, RCEIL, true, REAL_TYPES},
    {"floor", 1, MT_REAL, RFLOOR, true, REAL_TYPES},
    {"round", 1, MT_REAL, RROUND, true, REAL_TYPES},
    {"pi", 0, MT_REAL, RCONST_PI, true, NULL},
    {"slen", 1, MT_INT64, SLEN, true, STR_TYPES},
};

// TODO: inc and dec for integer and real types need passing address of the variable to the builtin function
// so that they so a inplace operation on the variable

// TODO: make a general len instead of slen ...


#define BUILTIN_COUNT (sizeof(BUILTIN_FUNCTIONS) / sizeof(BUILTIN_FUNCTIONS[0]))

const builtin_func_t* builtin_lookup(const char* name)
{
    for (size_t i = 0; i < BUILTIN_COUNT; i++)
    {
        if (strcmp(BUILTIN_FUNCTIONS[i].name, name) == 0)
        {
            return &BUILTIN_FUNCTIONS[i];
        }
    }
    return NULL;
}

bool_t builtin_is_reserved(const char* name)
{
    return builtin_lookup(name) != NULL;
}


#include "builtin.h"
#include <string.h>

// Acceptable type arrays for builtin functions
static const type_t INTEGER_TYPES[] = {MT_INT8, MT_INT16, MT_INT32, MT_INT64, MT_UNKNOWN};
static const type_t REAL_TYPES[] = {MT_REAL, MT_UNKNOWN};
static const type_t STR_TYPES[] = {MT_STR, MT_UNKNOWN};

// Builtin function registry
static const builtin_func_t BUILTIN_FUNCTIONS[] = {
    {"abs", 1, MT_INT64, IABS, true, INTEGER_TYPES},
    {"inc", 1, MT_INT64, IINC, true, INTEGER_TYPES},
    {"dec", 1, MT_INT64, IDEC, true, INTEGER_TYPES},
    {"pow", 2, MT_INT64, IPOW, true, INTEGER_TYPES},
    {"rabs", 1, MT_REAL, RABS, true, REAL_TYPES},
    {"rmod", 2, MT_REAL, RMOD, true, REAL_TYPES},
    {"rpow", 2, MT_REAL, RPOW, true, REAL_TYPES},
    {"rsqrt", 1, MT_REAL, RSQRT, true, REAL_TYPES},
    {"rexp", 1, MT_REAL, REXP, true, REAL_TYPES},
    {"rsin", 1, MT_REAL, RSIN, true, REAL_TYPES},
    {"rcos", 1, MT_REAL, RCOS, true, REAL_TYPES},
    {"rtan", 1, MT_REAL, RTAN, true, REAL_TYPES},
    {"racos", 1, MT_REAL, RACOS, true, REAL_TYPES},
    {"ratan2", 2, MT_REAL, RATAN2, true, REAL_TYPES},
    {"rlog", 1, MT_REAL, RLOG, true, REAL_TYPES},
    {"rlog10", 1, MT_REAL, RLOG10, true, REAL_TYPES},
    {"rlog2", 1, MT_REAL, RLOG2, true, REAL_TYPES},
    {"rceil", 1, MT_REAL, RCEIL, true, REAL_TYPES},
    {"rfloor", 1, MT_REAL, RFLOOR, true, REAL_TYPES},
    {"rround", 1, MT_REAL, RROUND, true, REAL_TYPES},
    {"slen", 1, MT_INT64, SLEN, true, STR_TYPES},
};

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


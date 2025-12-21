#ifndef BUILTIN_H
#define BUILTIN_H

#include "types.h"
#include "vm.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    const char* name;
    uint8_t arg_count;
    type_t ret_type;
    uint8_t opcode;  // VM opcode to emit, or 0 if not applicable
    bool_t is_builtin;  // Always true for builtin functions
    const type_t* acceptable_types;  // Array of acceptable argument types, terminated by MT_UNKNOWN
} builtin_func_t;

// Lookup a builtin function by name
const builtin_func_t* builtin_lookup(const char* name);

// Check if a name is a builtin function (for name collision prevention)
bool_t builtin_is_reserved(const char* name);

#ifdef __cplusplus
}
#endif

#endif /* BUILTIN_H */


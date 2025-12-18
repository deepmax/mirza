#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef double real_t;

typedef enum
{
    MT_UNKNOWN,
    MT_VOID,
    MT_STR,
    MT_INT,
    MT_BOOL,
    MT_LONG,
    MT_SHORT,
    MT_BYTE,
    MT_REAL,
    MT_FUNC,
    MT_LIST,
} mirza_type_t;

typedef union
{
    char* as_str;
    uint8_t as_byte;
    bool as_bool;
    real_t as_real;
    int16_t as_short;
    int32_t as_int;
    int64_t as_long;
    uint32_t as_uint;
    uint16_t as_ushort;
    uint64_t as_ulong;
    uintptr_t as_ptr;
} mirza_value_t;

typedef enum
{
    MB_NORMAL,
    MB_LOOP,
    MB_FUNC,
    MB_CLASS,
    MB_GLOBAL,
} mirza_block_t;

#ifdef __cplusplus
}
#endif

#endif /* TYPES_H */

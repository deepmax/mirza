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
typedef bool bool_t;
typedef char char_t;

typedef enum
{
    MT_UNKNOWN,
    MT_VOID,
    MT_BOOL,
    MT_STR,
    MT_INT32,
    MT_INT64,
    MT_INT16,
    MT_INT8,
    MT_UINT8,
    MT_UINT16,
    MT_UINT32,
    MT_UINT64,
    MT_REAL,
    MT_FUNC,
    MT_LIST,
} type_t;

typedef union
{
    char_t* as_str;
    bool_t as_bool;
    real_t as_real;
    int8_t as_int8;
    int16_t as_int16;
    int32_t as_int32;
    int64_t as_int64;
    uint8_t as_uint8;
    uint16_t as_uint16;
    uint32_t as_uint32;
    uint64_t as_uint64;
    uintptr_t as_ptr;
} value_t;

typedef enum
{
    MB_NORMAL,
    MB_LOOP,
    MB_FUNC,
    MB_CLASS,
    MB_GLOBAL,
} block_t;

#ifdef __cplusplus
}
#endif

#endif /* TYPES_H */

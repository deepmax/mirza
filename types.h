#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef char char_t;
typedef double real_t;
typedef int32_t integer_t;
typedef int64_t longint_t;
typedef int16_t short_t;
typedef uint32_t uinteger_t;
typedef uint64_t ulongint_t;
typedef uint16_t ushort_t;
typedef uint8_t byte_t;
typedef bool bool_t;

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
    char_t* as_str;
    byte_t as_byte;
    bool_t as_bool;
    real_t as_real;
    short_t as_short;
    integer_t as_int;
    longint_t as_long;
    uinteger_t as_uint;
    ushort_t as_ushort;
    ulongint_t as_ulong;
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

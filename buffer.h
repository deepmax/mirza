#ifndef BUFFER_H
#define BUFFER_H

#include "types.h"
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
    byte_t* data;
    size_t allc;
    size_t used;
} buffer_t;

void buffer_init(buffer_t* array, size_t size);
void buffer_free(buffer_t* array);
void buffer_clear(buffer_t* array);
void buffer_shrink(buffer_t* array);
void buffer_set(buffer_t* array, size_t index, byte_t item);
void buffer_sets(buffer_t* array, size_t index, byte_t* items, size_t len);
byte_t buffer_get(buffer_t* array, size_t index);
size_t buffer_size(buffer_t* array);
size_t buffer_add(buffer_t* array, byte_t item);
size_t buffer_adds(buffer_t* array, byte_t* items, size_t len);

#ifdef __cplusplus
}
#endif

#endif /* BUFFER_H */

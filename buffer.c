#include "buffer.h"
#include <malloc.h>

void buffer_init(buffer_t* array, size_t size)
{
    array->data = malloc(sizeof (uint8_t) * size);
    array->allc = size;
    array->used = 0;
}

void buffer_free(buffer_t* array)
{
    free(array->data);
}

void buffer_set(buffer_t* array, size_t index, uint8_t item)
{
    array->data[index] = item;
}

void buffer_sets(buffer_t* array, size_t index, uint8_t* items, size_t len)
{
    for (size_t i = 0; i < len; i++)
        array->data[i + index] = items[i];
}

uint8_t buffer_get(buffer_t* array, size_t index)
{
    return array->data[index];
}

void buffer_shrink(buffer_t* array)
{
    array->data = realloc(array->data, array->used);
    array->allc = array->used;
}

void buffer_clear(buffer_t* array)
{
    array->data = realloc(array->data, 32);
    array->allc = 32;
    array->used = 0;
}

size_t buffer_size(buffer_t* array)
{
    return array->used;
}

size_t buffer_add(buffer_t* array, uint8_t item)
{
    if (array->used == array->allc)
    {
        array->allc *= 2;
        array->data = realloc(array->data, array->allc * sizeof (uint8_t));
    }
    array->data[array->used++] = item;

    return array->used;
}

size_t buffer_adds(buffer_t* array, uint8_t* items, size_t len)
{
    if (array->used + len >= array->allc)
    {
        array->allc = array->used + len;
        array->data = realloc(array->data, array->allc * sizeof (uint8_t));
    }

    for (size_t i = 0; i < len; i++)
        array->data[array->used++] = items[i];

    return array->used;
}

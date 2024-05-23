#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct list_node_t
{
    struct list_node_t* next;
    struct list_node_t* prev;
    void* data;
} list_node_t;

typedef struct
{
    list_node_t* head;
    list_node_t* tail;
    size_t size;
} list_t;

list_t* list_new();
list_t* list_clone(list_t* list);
void list_free(list_t* list);
void list_clear(list_t* list);
void list_append(list_t* list, void* data);
void list_prepend(list_t* list, void* data);
void list_insert(list_t* list, size_t index, void* data);
void list_remove(list_t* list, size_t index);


void list_insert_next(list_t* list, list_node_t* node, void* data);
void list_insert_prev(list_t* list, list_node_t* node, void* data);
void list_remove_node(list_t* list, list_node_t* node);

void list_reverse(list_t* list);
void list_set(list_t* list, size_t index, void* data);
void* list_get(list_t* list, size_t index);
void* list_first(list_t* list);
void* list_last(list_t* list);
size_t list_size(list_t* list);
//void list_recalc_size(list_t* list);


list_node_t* node_walk_forward(list_node_t* node, size_t steps);
list_node_t* node_walk_backward(list_node_t* node, size_t steps);


#ifdef __cplusplus
}
#endif

#endif /* VECTOR_H */

#include "list.h"
#include <stdlib.h>

list_node_t* node_new(void* data)
{
    list_node_t* node = malloc(sizeof (list_node_t));
    node->data = data;
    node->prev = NULL;
    node->next = NULL;
    return node;
}

void node_free(list_node_t* node)
{
    free(node);
}

list_node_t* node_walk_forward(list_node_t* node, size_t steps)
{
    if (steps == 0)
        return node;

    while (steps > 0 && node->next)
    {
        node = node->next;
        steps--;
    }

    if (steps > 0)
        return NULL;

    return node;
}

list_node_t* node_walk_backward(list_node_t* node, size_t steps)
{
    if (steps == 0)
        return node;

    while (steps > 0 && node->prev)
    {
        node = node->prev;
        steps--;
    }

    if (steps > 0)
        return NULL;

    return node;
}

list_t* list_new()
{
    list_t* list = malloc(sizeof (list));
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
    return list;
}

list_t* list_clone(list_t* list)
{
    return NULL;
}

void list_free(list_t* list)
{
    list_clear(list);
    free(list);
}

void list_clear(list_t* list)
{
    while (list->size > 0)
        list_remove(list, 0);
}

void list_append(list_t* list, void* data)
{
    list_node_t* node = node_new(data);

    node->prev = list->tail;

    if (list->tail != NULL)
        list->tail->next = node;

    list->tail = node;

    if (list->head == NULL)
        list->head = node;

    list->size++;
}

void list_prepend(list_t* list, void* data)
{
    list_node_t* node = node_new(data);

    node->next = list->head;

    if (list->head != NULL)
        list->head->prev = node;

    list->head = node;

    if (list->tail == NULL)
        list->tail = node;

    list->size++;
}

void list_insert(list_t* list, size_t index, void* data)
{
    if (index == 0)
        return list_prepend(list, data);

    if (index >= list->size)
        return list_append(list, data);

    list_node_t* next_node = node_walk_forward(list->head, index);
    list_node_t* node = node_new(data);

    node->next = next_node;
    node->prev = next_node->prev;
    next_node->prev->next = node;
    next_node->prev = node;

    list->size++;
}

void list_remove(list_t* list, size_t index)
{
    if (index >= list->size || list->size == 0)
        return;

    list_node_t* node = NULL;

    if (list->size == 1)
    {
        node = list->head;
        list->head = NULL;
        list->tail = NULL;
    }
    else if (index == 0)
    {
        node = list->head;
        list->head = list->head->next;
        list->head->prev = NULL;
    }
    else if (index == list->size - 1)
    {
        node = list->tail;
        list->tail = list->head->prev;
        list->tail->next = NULL;
    }
    else
    {
        node = node_walk_forward(list->head, index);
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    node_free(node);

    list->size--;
}

void list_set(list_t* list, size_t index, void* data)
{
    if (index >= list->size)
        return;

    node_walk_forward(list->head, index)->data = data;
}

void* list_get(list_t* list, size_t index)
{
    if (index >= list->size)
        return NULL;

    return node_walk_forward(list->head, index)->data;
}

void* list_first(list_t* list)
{
    return list->head->data;
}

void* list_last(list_t* list)
{
    return list->tail->data;
}

size_t list_size(list_t* list)
{
    return list->size;
}

void list_reverse(list_t* list)
{
    for (list_node_t* node = list->head; node != NULL; node = node->prev)
    {
        list_node_t* n = node->next;
        list_node_t* p = node->prev;

        node->next = p;
        node->prev = n;
    }

    list_node_t* tmp = list->head;
    list->head = list->tail;
    list->tail = tmp;
}

void list_insert_next(list_t* list, list_node_t* node, void* data)
{
}

void list_insert_prev(list_t* list, list_node_t* node, void* data)
{
    
}

void list_remove_node(list_t* list, list_node_t* node)
{
    if (list->head == NULL)
        return;
    
    if (node->prev == NULL && node->next == NULL)
        return;
    
    if (node->prev != NULL)
        node->prev->next = node->next;

    if (node->next != NULL)
        node->next->prev = node->prev;

    node_free(node);

    list->size--;
}


//void list_recalc_size(list_t* list)
//{
//    size_t size = 0;
//
//    for (list_node_t* n = list->head; n != NULL; n = n->next)
//        size++;
//
//    list->size = size;
//}

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "arg_list.h"

struct arglist *arglist_init(void)
{
    struct arglist *r = calloc(sizeof(struct arglist), 1);
    return r;
}
int arglist_push_front(struct arglist *list, char* value)
{
    struct arg *e = calloc(sizeof(struct arg), 1);
    if (!e)
        return 0;
    e->content = value;
    if (list->size++ == 0)
        list->tail = e;
    else
    {
        e->next = list->head;
        e->prev = NULL;
        list->head->prev = e;
    }
    list->head = e;
    return 1;
}
void arglist_print(const struct arglist *list)
{
    struct arg *e = list->head;
    for (size_t i = 0; i < list->size; i++)
    {
        printf("%d\n", e->content);
        e = e->next;
    }
}
int arglist_push_back(struct arglist *list, char* value)
{
    if (value < 0)
        return 0;
    struct arg *e = calloc(sizeof(struct arg), 1);
    if (e == 0)
        return 0;
    e->content = value;
    if (list->size++ == 0)
        list->head = e;
    else
    {
        e->next = NULL;
        e->prev = list->tail;
        list->tail->next = e;
    }
    list->tail = e;
    return 1;
}
size_t arglist_size(const struct arglist *list)
{
    return list->size;
}

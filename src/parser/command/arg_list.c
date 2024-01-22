#include "arg_list.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

struct arglist *arglist_init(void)
{
    struct arglist *r = calloc(sizeof(struct arglist), 1);
    return r;
}

void arglist_print(const struct arglist *list)
{
    struct arg *e = list->head;
    for (size_t i = 0; i < list->size; i++)
    {
        printf("[%s] '%s' %c\n", e->type == VAR ? "VAR" : "WRD", e->content,
               e->link_next ? '|' : '=');
        e = e->next;
    }
}
bool arglist_push_back(struct arglist *list, bool link_next, char *content,
                       enum arg_type type)
{
    struct arg *e = calloc(sizeof(struct arg), 1);
    if (!e)
        return false;
    e->content = content;
    e->link_next = link_next;
    e->type = type;
    if (list->size++ == 0)
        list->head = e;
    else
    {
        e->next = NULL;
        list->tail->next = e;
    }
    list->tail = e;
    return true;
}

void clean_arglist(struct arglist *list)
{
    struct arg *a = list->head;
    for (size_t i = 0; i < list->size; i++)
    {
        struct arg *n = a->next;
        free(a->content);
        free(a);
        a = n;
    }
}

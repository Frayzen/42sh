#include "expansion.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "tools/str/string.h"

struct expansion *expansion_init(void)
{
    struct expansion *r = calloc(sizeof(struct expansion), 1);
    return r;
}

void expansion_print(const struct expansion *exp)
{
    static const char *exp_type_lt[] = {
        [UNQUOTED_VAR] = "UNQOTD_VAR",
        [QUOTED_VAR] = "QUOTED_VAR",
        [STR_LITTERAL] = "STR_LITTRL",
    };
    struct expandable *e = exp->head;
    for (size_t i = 0; i < exp->size; i++)
    {
        printf("[%s] '%s' %c\n", exp_type_lt[e->type], e->content,
               e->link_next ? '|' : '=');
        e = e->next;
    }
}

void expansion_push_back(struct expansion *exp, struct expandable *item)
{
    if (exp->size++ == 0)
        exp->head = item;
    else
    {
        item->next = NULL;
        exp->tail->next = item;
    }
    exp->tail = item;
}

struct expandable *expandable_init(char *content, enum expand_type type,
                                   bool link_next)
{
    struct expandable *e = calloc(1, sizeof(struct expandable));
    if (!e)
        return false;
    e->content = content;
    e->link_next = link_next;
    e->type = type;
    return e;
}

void clean_expansion(struct expansion *exp)
{
    struct expandable *e = exp->head;
    while (e)
    {
        struct expandable *n = e->next;
        if (e->content)
            free(e->content);
        free(e);
        e = n;
    }
}

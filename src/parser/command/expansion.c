#include "expansion.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "tools/ast/ast.h"
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
        [QUOTED_STR] = "QUOTED_STR",
    };
    struct expandable *e = exp->head;
    while (e)
    {
        printf("[%s] '%s' (%p) %c\n", exp_type_lt[e->type], (char *)e->content,
               (void *)e, e->link_next ? '|' : '=');
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

struct expandable *expandable_init(void *content, enum expand_type type,
                                   int link_next)
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
        {
            if (IS_SUBCMD_TYPE(e->type))
                destroy_ast(e->content);
            else
                free(e->content);
        }
        free(e);
        e = n;
    }
}

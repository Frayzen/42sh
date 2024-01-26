#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command/expansion.h"
#include "env/env.h"
#include "env/vars/vars.h"
#include "tools/str/string.h"

//
// REGISTERING TOKENS
//

/***
 * Append the next arg found in exp_str to cmd considering its type and begin
 * @param type the type of the arg
 * @param cmd the command to append the new arg to
 * @param exp_str the expand string to read from
 * @return the begin of the next
 */
int register_expandable(struct expansion *exp, struct lex_str *exp_str,
                        size_t begin)
{
    size_t end = begin;
    enum expand_type type = exp_str->expand[begin];
    if (type == STR_LITTERAL)
    {
        while (end < exp_str->size && exp_str->expand[end] == type)
            end++;
    }
    else
    {
        while (end < exp_str->size && exp_str->expand[end] == type
               && exp_str->value[end] != '$')
            end++;
    }
    size_t size = end - begin;
    bool is_last = exp_str->size == end;
    char *str = strndup(exp_str->value + begin, size);
    struct expandable *e = expandable_init(str, type, !is_last);
    expansion_push_back(exp, e);
    return end;
}

void exp_register_str(struct expansion *exp, struct lex_str *str)
{
    size_t i = 0;
    if (str->size == 0)
    {
        expansion_push_back(exp,
                            expandable_init(strdup(""), STR_LITTERAL, false));
    }
    while (i < str->size)
    {
        // Skip the dollar if needed
        if (str->value[i] == '$' && str->expand[i] != STR_LITTERAL)
            i++;
        i = register_expandable(exp, str, i);
    }
    destroy_lex_str(str);
}

//
// EXPANSION
//

// return the new current
static struct expandable *expand_unquoted_var(struct expandable *cur)
{
    assert(cur->type == UNQUOTED_VAR);
    char *val = retrieve_var(cur->content);
    char *elem = strtok(val, " ");
    struct expandable *last = NULL;
    struct expandable *first = NULL;
    if (!elem)
    {
        last = expandable_init(val, STR_LITTERAL, cur->link_next);
        last->next = cur->next;
        return last;
    }
    while (elem)
    {
        struct expandable *new_string = malloc(sizeof(struct expandable));
        new_string->type = STR_LITTERAL;
        new_string->content = strdup(elem);
        new_string->link_next = false;
        if (last)
            last->next = new_string;
        else
            first = new_string;
        last = new_string;
        elem = strtok(NULL, " ");
    }
    last->link_next = cur->link_next;
    last->next = cur->next;
    free(val);
    return first;
}

static struct expandable *expand_quoted_var(struct expandable *cur)
{
    assert(cur->type == QUOTED_VAR);
    struct expandable *new_string = malloc(sizeof(struct expandable));
    new_string->type = STR_LITTERAL;
    new_string->link_next = cur->link_next;
    new_string->next = cur->next;
    new_string->content = retrieve_var(cur->content);
    return new_string;
}

static struct expandable *expand_str_litt(struct expandable *cur)
{
    assert(cur->type == STR_LITTERAL);
    struct expandable *new_string = malloc(sizeof(struct expandable));
    new_string->type = STR_LITTERAL;
    new_string->link_next = cur->link_next;
    new_string->next = cur->next;
    new_string->content = strdup(cur->content);
    return new_string;
}

struct expansion *create_str_list(struct expansion *old)
{
    struct expansion *exp = malloc(sizeof(struct expansion));
    memcpy(exp, old, sizeof(struct expansion));
    struct expandable **last = &exp->head;
    struct expandable *cur = exp->head;
    while (cur)
    {
        switch (cur->type)
        {
            case STR_LITTERAL:
                *last = expand_str_litt(cur);
                break;
            case QUOTED_VAR:
                *last = expand_quoted_var(cur);
                break;
            case UNQUOTED_VAR:
                *last = expand_unquoted_var(cur);
                break;
        }
        while ((*last)->next != cur->next)
            last = &(*last)->next;
        last = &(*last)->next;
        cur = cur->next;
    }
    // Recompute the size
    exp->size = 0;
    cur = exp->head;
    while (cur)
    {
        exp->size++;
        cur = cur->next;
    }
    return exp;
}

char **expand(struct expansion *expansion)
{
    struct expansion *exp = create_str_list(expansion);
    struct expandable *e = exp->head;
    int argc = 0;
    char **argv = NULL;
    // for all arg
    while (e)
    {
        argv = realloc(argv, sizeof(char *) * ++argc);
        char *cur = calloc(1, sizeof(char));
        size_t size = 0;
        bool link_next;
        // for all str in the current arg
        DBG_VAR("ARG[%d] ", argc - 1);
        do
        {
            link_next = e->link_next;
            DBG_VAR("%s ", e->content);
            if (link_next)
                printf(" -> ");
            size += strlen(e->content);
            cur = realloc(cur, sizeof(char) * (size + 1)); 
            strcat(cur, e->content);
            e = e->next;
        } while (e && link_next);
        DBG_VAR("\n");
        argv[argc - 1] = cur;
    }
    clean_expansion(exp);
    free(exp);
    argv = realloc(argv, sizeof(char *) * ++argc);
    argv[argc - 1] = NULL;
    return argv;
}

void destroy_expanded(char **argv)
{
    int i = 0;
    while (argv[i])
        free(argv[i++]);
    free(argv);
}

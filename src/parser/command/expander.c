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
    if (IS_STR_TYPE(type))
    {
        while (end < exp_str->size && exp_str->expand[end] == type)
            end++;
    }
    else
    {
        do
            end++;
        while (end < exp_str->size && exp_str->expand[end] == type
               && exp_str->value[end] != '$');
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
        if (str->value[i] == '$' && !IS_STR_TYPE(str->expand[i]))
            i++;
        i = register_expandable(exp, str, i);
    }
    destroy_lex_str(str);
}

//
// EXPANSION
//

// return the new current or NULL if the expansion is empty
static struct expandable *expand_unquoted_var(struct expandable *cur)
{
    assert(cur->type == UNQUOTED_VAR);
    char *val = retrieve_var(cur->content);
    if (val[0] == '\0')
    {
        free(val);
        return NULL;
    }
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
        struct expandable *new_string =
            expandable_init(strdup(elem), STR_LITTERAL, false);
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

// return the new current or NULL if the expansion is empty
static struct expandable *expand_quoted_var(struct expandable *cur)
{
    assert(cur->type == QUOTED_VAR);
    char *content = retrieve_var(cur->content);
    if (content[0] == '\0')
    {
        free(content);
        return NULL;
    }
    struct expandable *new_string =
        expandable_init(content, STR_LITTERAL, cur->link_next);
    new_string->next = cur->next;
    return new_string;
}

static struct expandable *expand_str(struct expandable *cur)
{
    assert(IS_STR_TYPE(cur->type));
    struct expandable *new_string =
        expandable_init(strdup(cur->content), STR_LITTERAL, cur->link_next);
    new_string->next = cur->next;
    return new_string;
}

struct expansion *create_str_list(struct expansion *old)
{
    struct expansion *exp = calloc(1, sizeof(struct expansion));
    struct expandable *last = NULL;
    struct expandable *ret = NULL;
    struct expandable *cur = old->head;
    while (cur)
    {
        switch (cur->type)
        {
        case QUOTED_VAR:
            ret = expand_quoted_var(cur);
            break;
        case UNQUOTED_VAR:
            ret = expand_unquoted_var(cur);
            break;
        default:
            ret = expand_str(cur);
            break;
        }
        if (ret == NULL)
        {
            if (last)
                last->link_next = last->link_next && cur->link_next;
            cur = cur->next;
            continue;
        }
        if (last == NULL)
            exp->head = ret;
        else
            last->next = ret;
        for (last = ret; last->next != cur->next; last = last->next)
            exp->size++;
        last->next = NULL;
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
                DBG_VAR(" -> ");
            size += strlen(e->content);
            cur = realloc(cur, sizeof(char) * (size + 1));
            strcat(cur, e->content);
            e = e->next;
        } while (e && link_next);
        DBG_VAR("\n");
        argv[argc - 1] = cur;
    }
    if (get_env_flag()->debug_env)
    {
        printf("BEF\n");
        expansion_print(expansion);
        printf("AFT\n");
        expansion_print(exp);
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

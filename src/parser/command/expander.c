#define _POSIX_C_SOURCE 200809L
#include "command/expander.h"

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

// The function create_str_list creates a linked list of string based on an
// expansion, It allocates the memory inside another expansion and expand any
// variable accordingly The returned expansion only contains string_litterals
struct expansion *create_str_list(struct expansion *old)
{
    // The function keeps expanding(and append) elements from the old expansion
    // to exp
    struct expansion *exp = calloc(1, sizeof(struct expansion));
    struct expandable *last = NULL; // last element expanded
    struct expandable *ret = NULL; // returned element after expansion
    struct expandable *cur = old->head; // The current elem of the old exp
    while (cur)
    {
        // We expand the current based on its type
        // Every expansion create the (list of) expandable and return the first
        // one The last element of the new list points to the cur->next
        // expandable
        ret = stringify_expandable(cur);
        // If the expansion of the variable returns an empty string, the
        // argument should not be taken in account
        if (ret == NULL)
        {
            if (last)
                last->link_next = last->link_next && cur->link_next;
            cur = cur->next;
            continue;
        }
        // Set up the head for the first one
        if (last == NULL)
            exp->head = ret;
        else
            last->next = ret;
        // Recompute the size
        for (last = ret; last->next && last->next != cur->next;
             last = last->next)
            exp->size++;
        // Last element points to NULL
        last->next = NULL;
        cur = cur->next;
    }
    exp->tail = last;
    return exp;
}

// This function will expand the expansion given as a list of str
// The returned char ** is meant to be freed as it will be allocated
// The returned char ** is meant to be NULL terminated
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
        expansion_print(expansion);
    // Cleanup the created list and add the NULL terminatig str
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

char *expand_str(struct expansion *exp)
{
    char **argv = expand(exp);
    if (!argv || !argv[0])
    {
        free(argv);
        return calloc(1, sizeof(char));
    }
    int i = 0;
    size_t size = 0;
    while (argv[i])
        size += strlen(argv[i++]) + 1;
    char *ret = calloc(size, sizeof(char));
    i = 0;
    while (argv[i + 1])
    {
        strcat(ret, argv[i]);
        strcat(ret, " ");
        i++;
    }
    strcat(ret, argv[i]);
    destroy_expanded(argv);
    return ret;
}

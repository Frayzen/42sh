#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env/env.h"
#include "env/vars/specials.h"
#include "env/vars/vars.h"
#include "parser/command/expansion.h"
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

// Expand the unquoted var (= expand it and split it by ' ')
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
    char *ifs = DEFAULT_IFS;
    if (is_set_var("IFS"))
        ifs = read_var("IFS");
    char *elem = strtok(val, ifs);
    struct expandable *last = NULL;
    struct expandable *first = NULL;
    if (!elem)
    {
        last = expandable_init(val, STR_LITTERAL, cur->link_next);
        last->next = cur->next;
        free(val);
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
        elem = strtok(NULL, ifs);
    }
    last->link_next = cur->link_next;
    last->next = cur->next;
    free(val);
    return first;
}

// Expand the quoted var (= expand it)
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

// Expand the string litteral (= duplicate it)
static struct expandable *expand_str(struct expandable *cur)
{
    assert(IS_STR_TYPE(cur->type));
    struct expandable *new_string =
        expandable_init(strdup(cur->content), STR_LITTERAL, cur->link_next);
    new_string->next = cur->next;
    return new_string;
}

// This function calls the appropriate subfunction in order to create a string
// litteral linked list from the currrent expandable
static struct expandable *stringify_expandable(struct expandable *cur)
{
    struct expandable *exp = expand_special_var(cur);
    if (exp)
        return exp;
    switch (cur->type)
    {
    case QUOTED_VAR:
        return expand_quoted_var(cur);
    case UNQUOTED_VAR:
        return expand_unquoted_var(cur);
    default:
        return expand_str(cur);
    }
}

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

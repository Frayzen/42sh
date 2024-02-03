#define _POSIX_C_SOURCE 200809L

#include "command/expander.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#include "command/expansion.h"
#include "env/context.h"
#include "env/env.h"
#include "env/vars/vars.h"
#include "grammar/rules.h"
#include "io_backend/io_streamers.h"
#include "tools/ast/ast.h"
#include "tools/str/string.h"
//
// REGISTERING TOKENS
//

// Find the end of the expand type starting at begin in exp_str
static int read_until(struct lex_str *exp_str, size_t begin)
{
    size_t end = begin;
    enum expand_type type = exp_str->expand[end];
    while (end < exp_str->size && type == exp_str->expand[end])
    {
        if (exp_str->value[end] == '$' && IS_VAR_TYPE(type))
            break;
        end++;
    }
    // In case of the end, include it
    if (end < exp_str->size && exp_str->expand[end] == SUB_CMD_END)
        end++;
    return end;
}

// NULL on error
void *create_expandable_content(struct lex_str *str, size_t b, size_t e)
{
    enum expand_type type = str->expand[b];
    char *s = strndup(str->value + b, e - b);
    if (IS_SUBCMD_TYPE(type))
    {
        struct context *old = new_context();
        io_streamer_string(s); // set the input cmd for the subcmd
        if (gr_input(AST_ROOT) == ERROR)
        {
            free(s);
            load_context(old);
            return NULL;
        }
        struct ast *ret = *AST_ROOT;
        *AST_ROOT = NULL;
        free(s);
        load_context(old);
        return ret;
    }
    else
        return s;
}

// return false in case of an error
int exp_register_str(struct expansion *exp, struct lex_str *str)
{
    size_t i = 0;
    if (str->size == 0)
    {
        expansion_push_back(exp,
                            expandable_init(strdup(""), STR_LITTERAL, false));
        destroy_lex_str(str);
        return true;
    }
    while (i < str->size)
    {
        // SKip $
        if (!IS_STR_TYPE(str->expand[i]))
            i++;
        // Get the end of the str for expandable
        size_t end = read_until(str, i);
        // Create the content (either ast* or char*)
        void *content = create_expandable_content(str, i, end);
        if (!end)
        {
            destroy_lex_str(str);
            return false;
        }
        // Create and push the new expandable
        struct expandable *e =
            expandable_init(content, str->expand[i], end != str->size);
        expansion_push_back(exp, e);
        i = end;
    }
    destroy_lex_str(str);
    return true;
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
            DBG_VAR("%s ", (char *)e->content);
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

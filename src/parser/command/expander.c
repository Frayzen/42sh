#include "env/vars/vars.h"
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "command/expansion.h"
#include "env/env.h"
#include "tools/ast/ast.h"
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

char *stringify_expandable(struct expandable *exp)
{
    if (exp->type == STR_LITTERAL)
        return strdup(exp->content);
    char *ret = retrieve_var(exp->content);
    if (!ret)
        ret = "";
    return strdup(ret);
}

struct expandable *expand_next(struct expandable *exp, char **str)
{
    if (!exp)
        return 0;
    char *build = NULL;
    int bsize = 0;
    while (true)
    {
        // Append the stringify of the current expandable
        char *str_exp = stringify_expandable(exp);
        int i = 0;
        while (str_exp[i])
        {
            build = realloc(build, sizeof(char) * ++bsize);
            build[bsize - 1] = str_exp[i++];
        }
        free(str_exp);
        if (!exp->link_next || !exp->next)
            break;
        exp = exp->next;
    }
    build = realloc(build, sizeof(char) * ++bsize);
    build[bsize - 1] = '\0';
    *str = build;
    return exp->next;
}

char **expand(struct expansion *expansion)
{
    struct expandable *exp = expansion->head;
    char **argv = NULL;
    char *next = NULL;
    int argc = 0;
    VERBOSE("Expanding...\n");
    do
    {
        argv = realloc(argv, sizeof(char *) * ++argc);
        exp = expand_next(exp, &next);
        argv[argc - 1] = next;
        VERBOSE("[ARG %d] %s\n", argc - 1, next);
    } while (exp);
    argv = realloc(argv, sizeof(char *) * ++argc);
    argv[argc - 1] = NULL;
    return argv;
}

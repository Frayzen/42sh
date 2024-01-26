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

char *stringify_expandable(struct expandable *exp, char ***last_exp)
{
    char *first = NULL;
    if (exp->type == STR_LITTERAL)
        return strdup(exp->content);
    char *ret = retrieve_var(exp->content);
    // printf("ret = %s\n", ret) ;
    if (!ret)
        ret = "";
    // if we have an unquted var we may need to
    // split the string by spaces
    else if (exp->type == UNQUOTED_VAR)
    {
        // printf("enter unquted\n");
        first = strtok(ret, " ");
        // printf("first = %s\n", first);
        size_t size = 1;
        char *cur = strtok(NULL, " ");
        // printf("cur = %s\n", cur);
        if (!cur) // if there is only one word return it
            goto finish;
        first = strdup(first);
        char **temp = *last_exp;
        while (cur != NULL) // save the rest of the words in last_exp
        {
            temp = realloc(temp, sizeof(char *) * (size + 1));
            temp[size - 1] = strdup(cur);
            size++;
            cur = strtok(NULL, " "); // NULL terminate
        }
        temp[size - 1] = NULL;
        *last_exp = temp;
        return first;
    }
finish:
    return strdup(ret);
}

struct expandable *expand_next(struct expandable *exp, char **str)
{
    static char **last_exp = NULL;

    char *build = NULL;
    // Return the last_exp elements at each call till its empty
    if (last_exp)
    {
        static int id = 0;
        *str = last_exp[id++];
        if (last_exp[id])
        {

            // dont go to next exp if there are still strings in last_exp
            return exp;
        }
        build = *str;
        // Free once empty
        id = 0;
        free(last_exp);
        last_exp = NULL;
    }
    // last_exp is empty here
    if (!exp)
        return 0;
    int bsize = 0;
    VERBOSE("[EXPAND] ");
    while (true)
    {
        VERBOSE("%s%s ", exp->type == STR_LITTERAL ? "" : "$", exp->content);
        // Append the stringify of the current expandable and
        // fill last_exp with pending chars
        char *str_exp = stringify_expandable(exp, &last_exp);
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
    VERBOSE("TO '%s'\n", build);
    return last_exp ? exp : exp->next;
}

char **expand(struct expansion *expansion)
{
    struct expandable *exp = expansion->head;
    char **argv = NULL;
    char *next = NULL;
    int argc = 0;
    do
    {
        argv = realloc(argv, sizeof(char *) * ++argc);
        exp = expand_next(exp, &next);
        argv[argc - 1] = next;
    } while (exp);
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

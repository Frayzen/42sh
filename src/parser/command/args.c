#define _POSIX_C_SOURCE 200809L
#include "args.h"

#include <stdio.h>
#include <string.h>

#include "parser/command/arg_list.h"
#include "tools/ast/ast.h"
#include "tools/str/string.h"

// Append the var arg to the cmd command list
//@param i is the id of the first char
//@return the next id that is not part of the var name
int append(enum expand_type type, struct ast_cmd *cmd, struct exp_str *arg,
           size_t begin)
{
    size_t end = begin;
    if (type == STR_LITTERAL)
    {
        while (end < arg->size && arg->expand[end] == type)
            end++;
    }
    else
    {
        while (end < arg->size && arg->expand[end] == type && arg->value[end] != '$')
            end++;
    }
    size_t size = end - begin;
    bool is_last = arg->size == end;
    char *str = strndup(arg->value + begin, size);
    arglist_push_back(&cmd->arglist, !is_last, str, type);
    return end;
}

void parse_arg(struct ast_cmd *cmd, struct exp_str *arg)
{
    size_t i = 0;
    while (i < arg->size)
    {
        // Skip the dollar if needed
        if (arg->value[i] == '$' && arg->expand[i] != STR_LITTERAL)
            i++;
        i = append(arg->expand[i], cmd, arg, i);
    }
    destroy_exp_str(arg);
}

char **build_argv(struct arglist *args)
{
    args = args + 1;
    // TODO check that each var name is a valid name
    return NULL;
}

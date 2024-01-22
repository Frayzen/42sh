#define _POSIX_C_SOURCE 200809L
#include "args.h"

#include <stdio.h>
#include <string.h>

#include "parser/command/arg_list.h"
#include "tools/ast/ast.h"
#include "tools/str/string.h"

//Append the next arg found in exp_str to cmd considering its type and begin
//@param type the type of the arg
//@param cmd the command to append the new arg to
//@param exp_str the expand string to read from
//@return the begin of the next
int append(enum arg_type type, struct ast_cmd *cmd, struct exp_str *exp_str,
           size_t begin)
{
    size_t end = begin;
    if (type == VAR)
    {
        while (end < exp_str->size && exp_str->expand[end] && exp_str->value[end] != '$')
            end++;
    }
    else
    {
        while (end < exp_str->size && !exp_str->expand[end])
            end++;
    }
    size_t size = end - begin;
    bool is_last = exp_str->size == end;
    char *str = strndup(exp_str->value + begin, size);
    arglist_push_back(&cmd->arglist, !is_last, str, type);
    return end;
}

void parse_arg(struct ast_cmd *cmd, struct exp_str *arg)
{
    size_t i = 0;
    while (i < arg->size)
    {
        if (arg->value[i] == '$' && arg->expand[i])
            i = append(VAR, cmd, arg, i + 1);
        else
            i = append(STR, cmd, arg, i);
    }
    destroy_exp_str(arg);
}

char **build_argv(struct arglist *args)
{
    args = args + 1;
    // TODO check that each var name is a valid name
    return NULL;
}

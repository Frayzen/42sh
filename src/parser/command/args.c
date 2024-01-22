#define _POSIX_C_SOURCE 200809L
#include "args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include <threads.h>

#include "env/env.h"
#include "parser/command/arg_list.h"
#include "tools/ast/ast.h"
#include "tools/str/string.h"

// Append the next arg found in exp_str to cmd considering its type and begin
//@param type the type of the arg
//@param cmd the command to append the new arg to
//@param exp_str the expand string to read from
//@return the begin of the next
int append(enum expand_type type, struct ast_cmd *cmd, struct exp_str *exp_str,
           size_t begin)
{
    size_t end = begin;
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
    arglist_push_back(&cmd->arglist, !is_last, str, type);
    return end;
}

void parse_arg(struct ast_cmd *cmd, struct exp_str *arg)
{
    size_t i = 0;
    if (arg->size == 0)
        append(STR_LITTERAL, cmd, arg, 0);
    while (i < arg->size)
    {
        // Skip the dollar if needed
        if (arg->value[i] == '$' && arg->expand[i] != STR_LITTERAL)
            i++;
        i = append(arg->expand[i], cmd, arg, i);
    }
    destroy_exp_str(arg);
}

// if discard_spaces is set, remove any extra space from the string
char *expand(char *var_name)
{
    var_name = var_name + 1;
    // TODO expand actually the var
    char *val = malloc(sizeof(char) * 7);
    val[0] = 'e';
    val[1] = ' ';
    val[2] = ' ';
    val[3] = 'h';
    val[4] = '\0';
    return val;
}

void append_to_argv(struct arg_builder *builder)
{
    // Append null char to current to terminate it
    builder->current =
        realloc(builder->current, ++(builder->cur_length) * sizeof(char));
    builder->current[builder->cur_length - 1] = '\0';

    VERBOSE("'%s' is appended as a new argument\n", builder->current);
    // Append current to argv
    builder->argv = realloc(builder->argv, ++(builder->argc) * sizeof(char *));
    builder->argv[builder->argc - 1] = builder->current;
    // Reset values of current
    builder->current = NULL;
    builder->cur_length = 0;
}

void append_to_builder(struct arg_builder *builder, char *val,
                       enum expand_type type)
{
    int i = 0;
    char last = '\0';
    while (val[i])
    {
        if (val[i] == ' ' && type == UNQUOTED_VAR)
        {
            if (last != ' ')
                append_to_argv(builder);
        }
        else
        {
            builder->current = realloc(builder->current,
                                       ++(builder->cur_length) * sizeof(char));
            builder->current[builder->cur_length - 1] = val[i];
        }
        last = val[i];
        i++;
    }
}

char **build_argv(struct arglist *args)
{
    struct arg_builder builder = { 0 };
    struct arg *arg = args->head;
    while (arg)
    {
        enum expand_type type = arg->type;
        // Retrieve the content
        char *value = NULL;
        if (type == STR_LITTERAL)
            value = strdup(arg->content);
        else
            value = expand(arg->content);
        VERBOSE("'%s' is expanded as '%s'\n", arg->content, value);
        // Append it
        append_to_builder(&builder, value, type);
        if (!arg->link_next)
            append_to_argv(&builder);
        free(value);
        arg = arg->next;
    }
    // Make argv NULL terminated
    builder.argv = realloc(builder.argv, builder.argc * sizeof(char *));
    builder.argv[builder.argc] = NULL;
    return builder.argv;
}

void destroy_argv(char **argv)
{
    int i = 0;
    while (argv[i])
    {
        free(argv[i]);
        i++;
    }
    free(argv);
}

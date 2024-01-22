#define _POSIX_C_SOURCE 200809L
#include "args.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <threads.h>

#include "env/env.h"
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
        while (end < arg->size && arg->expand[end] == type
               && arg->value[end] != '$')
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
    bool last_space = false;
    while (val[i])
    {
        if (type == UNQUOTED_VAR && val[i] == ' ' && !last_space)
        {
            last_space = true;
            append_to_argv(builder);
            continue;
        }
        last_space = false;
        builder->current =
            realloc(builder->current, ++(builder->cur_length) * sizeof(char));
        builder->current[builder->cur_length - 1] = val[i];
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

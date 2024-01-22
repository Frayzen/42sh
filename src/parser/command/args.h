#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>

#include "tools/ast/ast.h"

struct arg_builder
{
    char **argv;
    int argc;
    char *current;
    int cur_length;
};

/***
 * append_arg: append the argument to the end of the current in command
 * @param cmd the command ast
 * @param arg the new argument to append
 */
void parse_arg(struct ast_cmd *cmd, struct exp_str *arg);

/***
 * Build the argv from the arg_list
 * @param list the arg_list structure
 * @return the argv expanded
 */
char **build_argv(struct arglist *args);
/***
 * Free the arguments in argv and argv itself, should be called with the return
 * value of build_argv
 * @param argv the arguments allocated
 */
void destroy_argv(char **argv);

#endif /* !ARGS_H */

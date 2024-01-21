#ifndef ARGS_H
#define ARGS_H

#include <stdbool.h>
#include "tools/ast/ast.h"

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

#endif /* !ARGS_H */

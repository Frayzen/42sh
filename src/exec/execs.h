#ifndef EXECS_H
#define EXECS_H

#include "tools/ast/ast.h"

/***
 * executes any of the basic functions
 * @param ast the tree to execute
 */
int exec_basic_function(struct ast *ast);

/***
 * executes the echo function
 * @param ast the tree to execute
 */
int exec_echo(struct ast *ast);

/***
 * executes the command ast
 * @param ast the AST_COMMAND to execute
 */
int exec_command(struct ast *ast);

/***
 * executes the list ast
 * @param ast the AST_LIST tree to execute
 */
int exec_list(struct ast *ast);

/***
 * Entry point to execute the given ast
 * @param ast tree to execute
 */
int exec_entry(struct ast *ast);

/***
 * executes the if ast
 * * @param ast tree to execute
 */
int exec_condition(struct ast *ast);

#endif /* EXECS_H */

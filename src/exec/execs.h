#ifndef EXEC_ECHO_H
#define EXEC_ECHO_H

#include "tools/ast/ast.h"

/***
 * executes the echo function
 * @param ast the tree to execute
 */
void exec_echo(struct ast *ast);

/***
 * executes the command ast
 * @param ast the AST_COMMAND to execute
 */
void exec_command(struct ast *ast);

/***
 * executes the list ast
 * @param ast the AST_LIST tree to execute
 */
void exec_list(struct ast *ast);

/***
 * Entry point to execute the given ast
 * @param ast tree to execute
 */
void exec_entry(struct ast *ast);

#endif /* EXEC_ECHO_H */

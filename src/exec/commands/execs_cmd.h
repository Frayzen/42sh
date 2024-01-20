#ifndef EXECS_CMD_H
#define EXECS_CMD_H

#include "tools/ast/ast.h"

/***
 * execute a builtin command
 * @param ast the AST_CMD tree to execute
 * @return the return value
 * @see exec_bin for other commands execution
 */
int exec_builtin(struct ast_cmd *ast);

/***
 * fork the program and executes the command
 * @param ast the AST_CMD tree to execute
 * @return the pid or -1 in case of error
 * @see exec_builtin for builtin execution
 */
int exec_bin(struct ast_cmd *ast);

/***
 * executes the pipe ast
 * @param ast tree to execute
 */
int exec_pipe(struct ast_pipe *ast);

#endif /* !EXECS_CMD_H */

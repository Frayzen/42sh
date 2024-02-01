#ifndef EXECS_CMD_H
#define EXECS_CMD_H

#include "tools/ast/ast.h"

#define PID_SET -1

/***
 * execute a command
 * @param ast the AST_CMD tree to execute
 * @param pid a pointer to the pid to be set in case of fork
 * @return either set the pid and return PID_SET or return the value
 */
int exec_cmd(struct ast_cmd *ast, int *pid);

/***
 * executes the pipe ast
 * @param ast tree to execute
 */
int exec_pipe(struct ast_pipe *ast);

/***
 * executes the subshell ast
 * @param ast tree to execute
 */
int exec_subshell(struct ast_subshell *ast);

/***
 * executes the shell command ast
 * @param ast tree to execute
 */
int exec_sh(struct ast_sh *sh);

#endif /* !EXECS_CMD_H */

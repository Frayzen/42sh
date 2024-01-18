#ifndef EXECS_H
#define EXECS_H

#include "tools/ast/ast.h"

struct sh_command
{
    struct ast *root;
    int redirs_fds[3];
    char **argv;
    int argc;
};

/***
 * build the command list based on the root of cmd
 * @param cmd the command list initialized with the root and each fds assigned
 * to themselves
 * @return true if everything went fine
 */
bool build_command(struct sh_command *cmd);

/***
 * Execute the sh_command previously built
 * @param command the sh_command to execute
 * @return the return value of the command
 * @see build_comman()
 */
int exec_sh_command(struct sh_command *command);

/***
 * executes the command ast
 * @param ast the AST_CMD tree to execute
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

/***
 * executes the not ast
 * @param ast tree to execute
 */
int exec_negate(struct ast *ast);

/***
 * executes the pipe ast
 * @param ast tree to execute
 */
int exec_pipe(struct ast *ast);

#endif /* EXECS_H */

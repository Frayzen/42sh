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

struct sh_command *build_command(struct ast *ast);

/***
 * executes the command ast
 * @param ast the AST_CMD tree to execute
 */
int exec_command(struct ast_cmd *ast);

/***
 * executes the list ast
 * @param ast the AST_LIST tree to execute
 */
int exec_list(struct ast_list *ast);

/***
 * Entry point to execute the given ast
 * @param ast tree to execute
 */
int exec_entry(struct ast *ast);

/***
 * executes the if ast
 * * @param ast tree to execute
 */
int exec_condition(struct ast_if *ast);

/***
 * executes the pipe ast
 * @param ast tree to execute
 */
int exec_pipe(struct ast_pipe *ast);

/***
 * executes the loop ast (either WHILE or UNTIL)
 * @param ast tree to execute
 */
int exec_loop(struct ast_loop *ast);

#endif /* EXECS_H */

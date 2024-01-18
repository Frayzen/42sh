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

/***
 * executes the not ast
 * @param ast tree to execute
 */
int exec_negate(struct ast *ast);

#endif /* EXECS_H */

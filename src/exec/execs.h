#ifndef EXECS_H
#define EXECS_H

#include "tools/ast/ast.h"

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
 * executes the loop ast (either WHILE or UNTIL)
 * @param ast tree to execute
 */
int exec_loop(struct ast_loop *ast);

/***
 * executes the for ast
 * @param ast tree to execute
 */
int exec_for(struct ast_for *ast);

#endif /* EXECS_H */

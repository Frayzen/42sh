#ifndef EXECS_H
#define EXECS_H

#include "tools/ast/ast.h"

#define NB_LOOPS (get_nb_loop(0))
#define CONTINUE (get_continue(0))
#define BREAK (get_break(0))

int get_nb_loop(int set);
int get_continue(int set);
int get_break(int set);

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

/***
 * executes the and_or ast
 * @param ast tree to execute
 */
int exec_and_or(struct ast_and_or *ast);

#endif /* EXECS_H */

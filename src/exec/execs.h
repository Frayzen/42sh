#ifndef EXECS_H
#define EXECS_H

#include "tools/ast/ast.h"

// get the number of loops we are currently in
#define NB_LOOPS (set_nb_loop(-1))
// get the number of remaining loops to continue
#define CNTU_LAYER (set_continue(-1))
// get the number of remaining loops to break
#define BREAK_LAYER (set_break(-1))
// indicates if the current loop is the loop to break or continue
#define IS_CUR_LOOP(Val) ((Val) == 0)

int set_nb_loop(int set);
int set_continue(int set);
int set_break(int set);

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

/***
 * executes the case ast
 * @param ast tree to execute
 */
int exec_case(struct ast_case *ast);

#endif /* EXECS_H */

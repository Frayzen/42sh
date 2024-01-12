#ifndef EXEC_ECHO_H
#define EXEC_ECHO_H

#include "tools/ast/ast.h"
#include "tools/token/token.h"



/***
 * echo_function: executes the echo function
 * @param ast the tree to execute
 */
void echo_function(struct ast *ast);

#endif /* EXEC_ECHO_H */

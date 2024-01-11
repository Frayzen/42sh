#ifndef AST_UTILS_H
#define AST_UTILS_H
#include "ast.h"
#include "token/token.h"


/***
* add_child: add a token to a tree
* @param src_ast: the source tree to add the new ast to
* @param new_ast: the tree to add to the source tree
* @return the source tree with the new ast as child
*/
struct ast *add_child(struct ast* src_ast,struct ast *new_ast);

#endif // AST_UTILS_H

#ifndef AST_UTILS_H
#define AST_UTILS_H
#include "ast.h"
#include "token/token.h"


/***
* add_child: add a token to a tree
* @param ast: the tree to add the token to
* @param token: the token to add to the tree
* @return the tree with the new token
*/
struct ast *add_child(struct ast* ast,struct token *token);

#endif // AST_UTILS_H

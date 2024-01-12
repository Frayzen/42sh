#ifndef AST_UTILS_H
#define AST_UTILS_H
#include "ast.h"
#include "token/token.h"

/***
 * add_child: add a token to a tree
 * @param parent: the parent tree that is receiving the child
 * @param child: the child tree that will be appended
 * @return the parent tree with child tree as child
 */
struct ast *add_child(struct ast *parent, struct ast *child);
#endif // AST_UTILS_H

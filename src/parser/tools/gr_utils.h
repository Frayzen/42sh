#ifndef GR_UTILS_H
#define GR_UTILS_H

#include "tools/ast/ast.h"

/***
 * append_arg: append the argument to the end of the current in command
 * @param cmd the command ast
 * @param arg the new argument to append
 */
void append_arg(struct ast_cmd *cmd, char *arg);

/***
 * add_child: add an ast to a tree list
 * @param parent: the parent tree that is receiving the child
 * @param child: the child tree that will be appended
 * @return the parent tree with child tree as child
 */
void add_child(struct ast_list *list, struct ast *child);

/***
 * Retrieve the children from any kind of ast
 * /!\ This function is for DEBUG purposes only
 * @return a static allocated array to the ast pointers
 */
struct ast **get_children(struct ast *ast);

#endif /* !GR_UTILS_H */

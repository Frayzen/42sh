#ifndef AST_UTILS_H
#define AST_UTILS_H
#include "tools/ast/ast.h"

/***
 * add_child: add a token to a tree
 * @param parent: the parent tree that is receiving the child
 * @param child: the child tree that will be appended
 * @return the parent tree with child tree as child
 */
struct ast *add_child(struct ast *parent, struct ast *child);

/***
 * Prints the AST
 * @param ast AST to be printed
 */
void pretty_print_ast(struct ast *ast);

/*
 * For the following AST :
 *
 * ast = {f1{a1 {b1, b2{c1 {d1}, c2}, b3{c3}}, a2, a3{b4, b5{c4{d2}}}}}
 *
 * Prints :
 *
 * f1
 * ╠══a1
 * ║  ╠══b1
 * ║  ╠══b2
 * ║  ║  ╠══c1
 * ║  ║  ║  ╠══d1
 * ║  ║  ╠══t2
 * ║  ╠══s3
 * ║  ║  ╚══t3
 * ╠══a2
 * ╚══a3
 *    ╠══b4
 *    ╚══b5
 *       ╚══c4
 *          ╚══d2
 */

#endif // AST_UTILS_H
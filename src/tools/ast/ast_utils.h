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

/***
 * Create a string from an AST /!\ Do not forget to free the returned str
 * @param ast AST to be stringified
 * @return the unique malloced string associated with the AST
 */
char *ast_to_str(struct ast *ast);

/***
 * Rebuild the instruction from the ast
 * @param ast the ast to recreate the instruction from
 * @param buffer a buffer large enough
 * @param i 0 initially
 */
int ast_rebuild(struct ast *ast, char *buffer, int i);

/***
 * Prints all three (pretty_print ast_to_str ast_rebuild) and print them
 * @param ast the ast to debug print
 */
void debug_pretty_print(struct ast *ast);

#endif // AST_UTILS_H

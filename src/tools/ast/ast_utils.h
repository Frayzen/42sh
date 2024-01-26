#ifndef AST_UTILS_H
#define AST_UTILS_H
#include "tools/ast/ast.h"
/***
 * Prints the AST with ast_to_str and pretty_print
 * @param ast AST to be printed
 */
void debug_pretty_print(struct ast *ast);

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
#endif // AST_UTILS_H

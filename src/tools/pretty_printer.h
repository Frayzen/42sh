#ifndef PRETTY_PRINTER
#define PRETTY_PRINTER

#include "ast/ast.h"

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

#endif /* !PRETTY_PRINTER */

#ifndef GRULES_H
#define GRULES_H

#include "tools/ast/ast.h"

/***
 * The grammar entry
 * @return the AST built
 */
struct ast *gr_entry(void);

/***
 * Grammar rule for element
 * @return the AST built
 */
struct ast *gr_element(void);

/***
 * Grammar rule for and_or
 * @return the AST built
 */
struct ast *gr_input(void);

/***
 * Grammar rule for and_or
 * @return the AST built
 */
struct ast *gr_list(void);

/***
 * Grammar rule for and_or
 * @return the AST built
 */
struct ast *gr_and_or(void);

#endif /* !GRULES_H */

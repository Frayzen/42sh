#ifndef GRULES_H
#define GRULES_H

#include "tools/ast/ast.h"

/***
 * The grammar entry
 * @return the AST built
 */
struct ast *grentry(void);

/***
 * Grammar rule for element
 * @return the AST built
 */
struct ast *grelement(void);

#endif /* !GRULES_H */

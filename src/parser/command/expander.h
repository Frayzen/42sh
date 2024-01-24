#ifndef EXPANDER_H
#define EXPANDER_H

#include "parser/command/expansion.h"
#include "tools/token/token.h"

/***
 * Register the token as part of the futur expansion of the command
 * @param exp the expansion
 * @param tok the token to register
 */
void register_token(struct expansion *exp, struct token *tok);

/***
 * Expand the expansion
 * @param expansion the expansion
 * @return the string list
 */
char **expand(struct expansion *expansion);

/***
 * Expand the expandable and return the size of the new str
 * @param exp the expandable
 * @param str the string. It is set to NULL if expandable is NULL
 * @return the next expandable
 */
struct expandable *expand_next(struct expandable *exp, char **str);

#endif /* !EXPANDER_H */

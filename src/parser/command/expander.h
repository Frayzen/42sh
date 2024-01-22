#ifndef EXPANDER_H
#define EXPANDER_H

#include "parser/command/expansion.h"
#include "tools/token/token.h"

/***
 * Register the token as part of the futur expansion of the command
 * @param cmd the command
 * @param tok the token to register
 */
void register_token(struct expansion *exp, struct token *tok);

/***
 * Expand the expansion
 * @param expansion the expansion
 * @return the string list
 */
char **expand(struct expansion *expansion);

#endif /* !EXPANDER_H */

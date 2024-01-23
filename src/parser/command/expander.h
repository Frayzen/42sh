#ifndef EXPANDER_H
#define EXPANDER_H

#include "parser/command/expansion.h"
#include "tools/str/string.h"
#include "tools/token/token.h"

/***
 * Register the token as part of the futur expansion of the command
 * @param cmd the command
 * @param tok the token to register
 */
void exp_register_str(struct expansion *exp, struct lex_str *str);

/***
 * Expand the expansion
 * @param expansion the expansion
 * @return the string list
 */
char **expand(struct expansion *expansion);

#endif /* !EXPANDER_H */

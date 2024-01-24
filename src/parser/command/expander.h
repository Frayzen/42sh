#ifndef EXPANDER_H
#define EXPANDER_H

#include "parser/command/expansion.h"
#include "tools/str/string.h"

/***
 * Register the token as part of the futur expansion of the command
 * @param exp the expansion
 * @param tok the token to register
 */
void exp_register_str(struct expansion *exp, struct lex_str *str);

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

/***
 * Free the arguments and the argv pointer
 * @param argv the argument list
 */
void destroy_expanded(char **argv);

#endif /* !EXPANDER_H */

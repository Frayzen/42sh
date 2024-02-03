#ifndef EXPANDER_H
#define EXPANDER_H

#include "parser/command/expansion.h"
#include "tools/str/string.h"

/***
 * Register the token as part of the futur expansion of the command
 * @param exp the expansion
 * @param tok the token to register
 * @return true if the registration happendened succesfully
 */
int exp_register_str(struct expansion *exp, struct lex_str *str);

/***
 * Stringify an expandable into several str_litt expandable linked list
 * @param cur the current expandable to expand
 * @return the list of the expanded str_litt
 */
struct expandable *stringify_expandable(struct expandable *cur);

/***
 * Expand the expansion
 * @param expansion the expansion
 * @return the string list
 */
char **expand(struct expansion *expansion);

/***
 * Expand the expansion to a single string
 * @param expansion the expansion
 * @return the allocated string
 */
char *expand_str(struct expansion *exp);

/***
 * Free the arguments and the argv pointer
 * @param argv the argument list
 */
void destroy_expanded(char **argv);

#endif /* !EXPANDER_H */

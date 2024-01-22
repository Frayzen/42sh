#ifndef EXPANDER_H
#define EXPANDER_H

#include "tools/ast/ast.h"

/***
 * Register the token as part of the futur expansion of the command
 * @param cmd the command
 * @param tok the token to register
 */
void cmd_register_token(struct ast_cmd *cmd, struct token *tok);

/***
 * Expand the expansion
 * @param expansion the expansion
 * @return the string list
 */
char **expand(struct expansion *expansion);

#endif /* !EXPANDER_H */

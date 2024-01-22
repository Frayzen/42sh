#ifndef LEXER_TOOLS_H
#define LEXER_TOOLS_H

#include <stdbool.h>
#include "tools/str/string.h"
#include "tools/token/token.h"

bool is_terminating(struct token *token);
bool chevron_type(const struct string *str);
bool assigment_word(const struct string *str);

#endif /* !LEXER_TOOLS_H */

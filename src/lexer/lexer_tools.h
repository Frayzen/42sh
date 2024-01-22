#ifndef LEXER_TOOLS_H
#define LEXER_TOOLS_H

#include <stdbool.h>
#include "lexer/finder.h"
#include "tools/str/string.h"
#include "tools/token/token.h"

bool is_terminating(struct token *token);
bool chevron_type(const struct string *str);
bool assignment_word(const struct string *str);
void skip_until(struct pending *p, char limit, bool append);
void append_char(struct pending *p);

#endif /* !LEXER_TOOLS_H */

#ifndef FINDER_TOOLS_H
#define FINDER_TOOLS_H

#include <stdbool.h>

#include "lexer/finder/finder.h"
#include "tools/str/string.h"
#include "tools/token/token.h"

/***
 * Check if a type is terminating
 * @param type the type to check
 * @return true if the token type is a terminating one
 */
bool is_terminating(enum token_type type);
/***
 * Check if a string matches one of the chevrons
 * @param str the string to check
 * @return true if the string matches
 */
bool chevron_type(const struct lex_str *str);
/***
 * Check if a string could be an assignment word
 * @param str the string to check
 * @return true if could
 */
bool assignment_word(const struct lex_str *str);

enum skip_behavior
{
    SKIP_SINGLE_QUOTE,
    SKIP_DOUBLE_QUOTE,
    SKIP_VARIABLE_BRACKETS,
    SKIP_HASHTAG,
    SKIP_PARENTHESES,
};

/***
 * pop the chars until the limit is found
 * @param p the pending structure to append char in
 * @param behavior the behavior of the skippping function
 */
void skip_until(struct pending *p, enum skip_behavior behavior);

/***
 * append a char to the pending, pop it as well
 * @param p the pending structure
 * @param c the char to append
 */
void append_char(struct pending *p, char c);

/***
 * append a char to the pending, pop it as well
 * @param p the pending structure
 */
void append_io(struct pending *p);
/***
 * Check if the char could be part of a name
 * @param c the char to test
 * @return true if the char might be in a name
 */
bool is_name_char(char c);

/***
 * Check if the given string is a valid name for a variable
 * @param str the buffer
 * @param size the size of the buffer
 * @return true if it is valid name
 */
bool is_name(char *str, size_t size);

void skip_sub_cmd(struct pending *p);
#endif /* !FINDER_TOOLS_H */

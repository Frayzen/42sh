#ifndef FINDER_TOOLS_H
#define FINDER_TOOLS_H


#include <stdbool.h>
#include "finder/finder.h"
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
bool chevron_type(const struct exp_str *str);
/***
 * Check if a string could be an assignment word
 * @param str the string to check
 * @return true if could
 */
bool assignment_word(const struct exp_str *str);
/***
 * pop the chars until the limit is found
 * @param p the pending structure to append char in
 * @param limit the limit char that will make the function return
 * @param append true if the popped character need to be appended
 */
void skip_until(struct pending *p, char limit, bool append);
/***
 * append a char to the pending, pop it as well
 * @param p the pending structure
 */
void append_char(struct pending *p);
/***
 * Check if the char is part of the portable character set
 * @param c the char to test
 * @return true if the char is
 */
bool is_portable_char(char c);

#endif /* !FINDER_TOOLS_H */


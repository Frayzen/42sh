#ifndef FINDER_H
#define FINDER_H

#include <stdbool.h>

#include "exit/error_handler.h"
#include "tools/str/string.h"

#define IS_BLANK(Pending) ((Pending)->blank)

#define SPECIAL_PARAMETERS "0123456789@*#?-$!0"

#define SPACE_CASES                                                            \
case ' ':                                                                      \
case '\f':                                                                     \
case '\r':                                                                     \
case '\t':                                                                     \
case '\v'

#define QUOTE_CASES                                                            \
case '\'':                                                                     \
case '"':                                                                      \
case '\\'

#define REDIR_OPS_CASES                                                        \
case '<':                                                                      \
case '>'

#define AND_OR_CASES                                                           \
case '&':                                                                      \
case '|'

#define CONTROL_OP_CASES                                                       \
AND_OR_CASES:                                                                  \
case ';':                                                                      \
case ')':                                                                      \
case '(':                                                                      \
case '\n'

#define OPERATORS_CASES                                                        \
REDIR_OPS_CASES:                                                               \
    CONTROL_OP_CASES

struct pending
{
    struct lex_str str;
    bool backslashed;
    bool blank;
    bool expanding;
    bool in_quote;
    bool error;
    bool force_str;
    bool in_sub_cmd;
};

// In this file, understand 'current character' as the one returned by
// io_peek

/***
 * finder: gets character by character and return the string token
 * @return a string struct
 * @see struct lex_str
 */
struct pending *finder(void);

/***
 * Append all the next characters of the variable after the current $
 * @param p the pending struct
 */
void consume_variable(struct pending *p);

/***
 * Handle the current character knowing it is an operator char
 * @param p the pending struct
 */
void consume_operators(struct pending *p);

/***
 * Exit gracefully or set p->error to true if exit fails (interactive mode)
 * @param type the type of the error
 */
void exit_lexer(enum error_type type);

#endif /* FINDER_H */

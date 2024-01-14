#ifndef TOKEN_H
#define TOKEN_H
#include <stdbool.h>

#include "lexer/finder.h"

#define IS_BUILTIN(t) ((t)->type >= T_TRUE && (t)->type <= ECHO)

// /!\ Do not add gaps inside of this enum (see TOK_TYPES_LT)
enum token_type
{
    // end of instruction
    NEWLINE,
    BSZERO,
    SEMI_COLON,

    // condition
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    // builtins /!\ leave T_TRUE as first one and ECHO as last one
    T_TRUE,
    T_FALSE,
    ECHO,
    // wildcard
    QUOTE,
    // terminals
    WORD,

    MAX_TOKEN = WORD
};

#define TOK_TYPES_LT (toktype_lookup())
#define TOK_TYPES_SIZE MAX_TOKEN

struct token
{
    enum token_type type;
    char *value;
    bool terminal;
};

/***
 * creates a token, sets value to lexime or 'term', sets type
 * @param value <- the string passed from the finder
 * @return <- a malloced token structure
 */
struct token *init_token(const struct string *str);

/***
 * frees a token
 * @param token<- the token to free
 */
void destroy_token(struct token *token);

/***
 * prints the token value and type number
 * @param token <- token to print
 */
void print_token(struct token *token);

/***
 * return the token type lookup table
 * @return The lookup table
 */
const char **toktype_lookup(void);

#endif // !TOKEN_H

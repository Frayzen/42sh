#ifndef TOKEN_H
#define TOKEN_H
#include <stdbool.h>

#include "lexer/finder.h"

#define IS_WORDABLE(t) ((t)->type >= IF)
#define IS_COMMAND(t) ((t)->type >= T_TRUE)
#define IS_OPERATOR(t) ((t)->type == AND || (t)->type == OR)

// /!\ Do not add gaps inside of this enum (see TOK_TYPES_LT)
enum token_type
{
    TOK_ERROR = -1,
    // end of instruction
    NEWLINE,
    BSZERO,
    SEMI_COLON,
    // redir
    CHEVRON,
    PIPE,
    // numbers
    EQUAL,
    // wildcard
    QUOTE,

    IO_NUMBER,
    AND,
    OR,
    // WORDABLES UNDER

    // condition
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,

    WHILE,
    UNTIL,
    DO,
    DONE,

    NEGATION,

    // builtins /!\ leave T_TRUE as first one and ECHO as last one
    T_TRUE,
    T_FALSE,
    ECHO,
    // terminals
    WORD,
};

#define TOK_TYPES_LT (toktype_lookup())

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

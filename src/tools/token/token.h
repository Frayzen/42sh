#ifndef TOKEN_H
#define TOKEN_H
#include <stdbool.h>

// /!\ Do not add gaps inside of this enum (see TOK_TYPE_LT)
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
    // builtins
    T_TRUE,
    T_FALSE,
    ECHO,
    // wildcard
    QUOTE,
    // terminals
    WORD,
};

struct token
{
    enum token_type type;
    char *value;
    bool terminal;
};

/***
 * creates a token, sets value to lexime or 'term', sets type
 * @param value <- the lexeme passed from the finder
 * @return <- a malloced token structure
 */
struct token *init_token(char *value);

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

#define TOK_TYPE_LT (toktype_lookup())
/***
 * return the token type lookup table
 * @return The lookup table
 */
const char **toktype_lookup(void);

/***
 * @param token: the toekn to check
 * @return if the token is one of the three builtins 
*/ 
bool is_builtin(struct token *token);

#endif // !TOKEN_H

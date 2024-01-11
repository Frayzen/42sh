#ifndef TOKEN_H
#define TOKEN_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NBTOKENS 13
enum token_type
{
    NEWLINE,
    BSZERO, 
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    SEMI_COLON,
    QUOTE,
    ECHO,
    TRUE,
    FALSE,
    WORD = NBTOKENS  //needs to be th last token in the enum for get_type()
};

struct token
{
    enum token_type type;
    char *value;
    bool terminal;
};

/***
 * creates a token, sets value to lexime or 'term', sets type
 * @param value the lexeme passed from the finder
 * @return a malloced token structure
 */
struct token *init_token(char *value);

/***
 * frees a token
 * @param token the token to free
 */
void destroy_token(struct token *token);

/***
 * prints the token value and type number
 * @param token token to print
 */
void print_token(struct token *token);

/***
 * checks if the token is a terminal character
 * @param token the token to check
 * @return 1 if the token is a terminal,
 * 0 if null or non terminal
 */
bool is_terminating(struct token *token);

#endif // !TOKEN_H

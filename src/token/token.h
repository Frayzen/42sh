#ifndef TOKEN_H
#define TOKEN_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
enum token_type
{
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    SEMI_COLON,
    NEWLINE,
    QUOTE,
    WORD,
    TERMINAL
};

struct token
{
    enum token_type type;
    char *value;
};
struct token *init_token(char *value);
void destroy_token(struct token *token);
void print_token(struct token *token);

#endif // !TOKEN_H

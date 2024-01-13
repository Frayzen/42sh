#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_terminating(struct token *token)
{
    switch (token->type)
    {
    case NEWLINE:
    case SEMI_COLON:
    case BSZERO:
        return true;
    default:
        return false;
    }
}

int get_type(char *value)
{
    int i = 0;
    while (TOK_TYPE_LT[i] && strcmp(TOK_TYPE_LT[i], value))
        i++;
    return i;
}

struct token *init_token(char *value)
{
    struct token *tok = malloc(sizeof(struct token));
    tok->type = get_type(value);
    tok->value = value;
    tok->terminal = is_terminating(tok);
    return tok;
}

void destroy_token(struct token *token)
{
    if (!token)
        return;
    if (token->value)
        free(token->value);
    free(token);
}

void print_token(struct token *token)
{
    if (!token)
        printf(" |NULL| ");
    else
        printf(" |%s|%d| ", token->value, token->type);
    printf("\n");
}

const char **toktype_lookup(void)
{
    static const char *lookup_table[] = {
        [IF] = "if",       [THEN] = "then",     [ELIF] = "elif",
        [ELSE] = "else",   [FI] = "fi",         [SEMI_COLON] = ";",
        [NEWLINE] = "\n",  [QUOTE] = "'",       [ECHO] = "echo",
        [T_TRUE] = "true", [T_FALSE] = "false", [BSZERO] = "\0",
        [WORD] = NULL,
    };
    return lookup_table;
}

bool is_builtin(struct token *token)
{
    enum token_type t = token->type;
    return t == ECHO || t == T_TRUE || t == T_FALSE;
}

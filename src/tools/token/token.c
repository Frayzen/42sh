#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *g_types_name[] = {
    [IF] = "if",     [THEN] = "then",    [ELIF] = "elif",     [ELSE] = "else",
    [FI] = "fi",     [SEMI_COLON] = ";", [NEWLINE] = "\n",    [QUOTE] = "'",
    [ECHO] = "echo", [T_TRUE] = "true",  [T_FALSE] = "false", [BSZERO] = "\0",
    [WORD] = NULL
};

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
    while (g_types_name[i] && strcmp(g_types_name[i], value))
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

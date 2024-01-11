#include "token.h"

char *g_elements[NBTOKENS] = {
    [IF] = "if",     [THEN] = "then",    [ELIF] = "elif",  [ELSE] = "else",
    [FI] = "fi",     [SEMI_COLON] = ";", [NEWLINE] = "\n", [QUOTE] = "'",
    [BSZERO] = "\0", [WORD] = NULL,      [TRUE] = "true",  [FALSE] = "false"
};

int is_terminating(struct token *token)
{
    if (!token)
        return 0;
    return token->type < 2;
}

int get_type(char *value)
{
    int i = 0;
    while (g_elements[i] && strcmp(g_elements[i], value))
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

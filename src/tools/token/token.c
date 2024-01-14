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

int get_type(const struct string *str)
{
    if (!str || str->size == 0)
        return WORD;
    int i = 0;
    while (TOK_TYPES_LT[i])
    {
        size_t c_id = 0;
        for (; c_id < str->size; c_id++)
{
            if (!TOK_TYPES_LT[i][c_id] || TOK_TYPES_LT[i][c_id] != str->value[c_id])        
                break;
        }
        if (c_id == str->size)
            return i;
        i++;
    }
    return i;
}

struct token *init_token(const struct string *str)
{
    struct token *tok = malloc(sizeof(struct token));
    tok->type = get_type(str);
    tok->value = str->value;
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

#define _XOPEN_SOURCE 700
#include "token.h"

#include <ctype.h>
#include <fnmatch.h>
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

bool chevron_type(const struct string *str)
{
    if (!strcmp(">", str->value))
        return 1;
    if (!strcmp("<", str->value))
        return 1;
    if (!strcmp(">>", str->value))
        return 1;
    if (!strcmp(">&", str->value))
        return 1;
    if (!strcmp("<&", str->value))
        return 1;
    if (!strcmp(">|", str->value))
        return 1;
    if (!strcmp("<>", str->value))
        return 1;
    return 0;
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
            if ((i != BSZERO && !TOK_TYPES_LT[i][c_id])
                || TOK_TYPES_LT[i][c_id] != str->value[c_id])
                break;
        }
        if (c_id == str->size)
            return i;
        i++;
    }
    if (chevron_type(str))
    {
        return CHEVRON;
    }
    // printf("valu = %s|\n", str->value);
    if (fnmatch("[0-9]+", str->value, 0) == 0)
        return IO_NUMBER;
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

char *to_upper(const char *type)
{
    if (!type)
        return NULL;
    char *str = strdup(type);
    size_t i = 0;
    while (str[i] != 0)
    {
        str[i] = toupper(str[i]);
        i++;
    }
    return str;
}

void print_token(struct token *token)
{
    if (!token)
        printf(" |NULL| ");
    else
    {
        const char **tok_type = toktype_lookup();
        const char *type_token = tok_type[token->type];
        char *type = to_upper(type_token);
        if (!type)
            printf(" |%s|%s| ", "WORD", token->value);
        else if (!strcmp(type, "\n"))
            printf(" |%s|%s| ", "NEWLINE", "\\n");
        else if (!strcmp(type, "\0"))
            printf(" |%s|%s| ", "BSZERO", "\\0");
        else
            printf(" |%s|%s| ", type, token->value);
        free(type);
    }
    printf("\n");
}

const char **toktype_lookup(void)
{
    static const char *lookup_table[] = {
        [IF] = "if",           [THEN] = "then",
        [ELIF] = "elif",       [ELSE] = "else",
        [FI] = "fi",           [SEMI_COLON] = ";",
        [NEWLINE] = "\n",      [QUOTE] = "'",
        [ECHO] = "echo",       [T_TRUE] = "true",
        [T_FALSE] = "false",   [BSZERO] = "\0",
        [CHEVRON] = "CHEVRON", [IO_NUMBER] = "[0-9]+",
        [EQUAL] = "=",         [WORD] = NULL,
    };
    return lookup_table;
}

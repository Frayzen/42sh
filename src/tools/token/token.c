#define _XOPEN_SOURCE 700
#include "tools/token/token.h"

#include <ctype.h>
#include <fnmatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "io_backend/backend_saver.h"
#include "lexer/finder/finder_tools.h"
#include "tools/str/string.h"

int get_type(const struct lex_str *str)
{
    if (!str || str->size == 0)
        return WORD;
    int i = 0;
    while (TOK_TYPES_LT[i])
    {
        size_t c_id = 0;
        const char *check_with = TOK_TYPES_LT[i];
        for (; c_id < str->size; c_id++)
        {
            if (i != BSZERO && !check_with[c_id])
                break;
            if (check_with[c_id] != str->value[c_id])
                break;
        }
        if (c_id == str->size && !check_with[c_id])
            return i;
        i++;
    }
    if (chevron_type(str))
        return CHEVRON;
    char next = io_peek();
    if (assignment_word(str))
        return ASSMT;
    if ((next == '>' || next == '<') && is_number(str->value))
        return IO_NUMBER;
    return i;
}

struct token *init_token(struct lex_str *str)
{
    struct token *tok = malloc(sizeof(struct token));
    tok->type = get_type(str);
    tok->str = dup_lex_str(str);
    tok->terminal = is_terminating(tok->type);
    return tok;
}

void destroy_token(struct token *token)
{
    if (!token)
        return;
    if (token->str)
        destroy_lex_str(token->str);
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

#define UNDER_EXPAND_CHAR(Type) (IS_VAR_TYPE(Type) ? '^' : ' ')
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
            printf(" |%s|%s| ", "WORD", token->str->value);
        else if (!strcmp(type, "\n"))
            printf(" |%s|%s| ", "NEWLINE", "\\n");
        else if (!strcmp(type, "\0"))
            printf(" |%s|%s| ", "BSZERO", "\\0");
        else
            printf(" |%s|%s| ", type, token->str->value);
        printf("\n");
        if (token->type == WORD || token->type == ASSMT)
        {
            printf("       ");
            for (size_t i = 0; i < token->str->size; i++)
                printf("%c", UNDER_EXPAND_CHAR(token->str->expand[i]));
        }
        free(type);
    }
    printf("\n");
}

const char **toktype_lookup(void)
{
    static const char *lookup_table[] = {
        [IF] = "if",         [THEN] = "then",
        [ELIF] = "elif",     [ELSE] = "else",
        [FI] = "fi",         [SEMI_COLON] = ";",
        [NEWLINE] = "\n",    [ECHO] = "echo",
        [T_TRUE] = "true",   [T_FALSE] = "false",
        [BSZERO] = "\0",     [CHEVRON] = "CHEVRON",
        [IO_NUMBER] = "NB",  [NEGATION] = "!",
        [PIPE] = "|",        [WHILE] = "while",
        [DO] = "do",         [DONE] = "done",
        [UNTIL] = "until",   [OR] = "||",
        [AND] = "&&",        [FOR] = "for",
        [IN] = "in",         [UNSET] = "unset",
        [DOT] = ".",         [ASSMT] = "ASMT",
        [BRK_OPEN] = "{",    [BRK_CLOSED] = "}",
        [PRTH_OPEN] = "(",   [PRTH_CLOSED] = ")",
        [WORD] = NULL,       [T_CONTINUE] = "continue",
        [T_BREAK] = "break", [DBL_SEMI_COLON] = ";;",
        [AMPERSAND] = "&",
    };
    return lookup_table;
}

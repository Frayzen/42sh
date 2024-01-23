#define _POSIX_C_SOURCE 200809L
#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>
#include <stdlib.h>
#include <string.h>

#include "tools/token/token.h"

void check_token(struct token *token, struct lex_str *str, enum token_type type)
{
    cr_assert_neq(token, NULL, "token is null");
    cr_assert(strcmp(token->value, str->value) == 0, "incorrect value");
    cr_assert_eq(type, token->type, "incorrect type is %d expected %d",
                 token->type, type);
    destroy_token(token);
}

struct lex_str *str_create(char *value)
{
    static struct lex_str str;
    str.size = strlen(value);
    str.value = strdup(value);
    return &str;
}

Test(NewlineToken, init_newline_token)
{
    char *str = "\n";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, NEWLINE);
}
Test(BSZeroToken, init_bszero_token)
{
    char *str = "\0";
    struct lex_str *value = str_create(str);
    value->size = 1;
    struct token *token = init_token(value);
    check_token(token, value, BSZERO);
}

Test(Token, init_if)
{
    char *str = "if";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, IF);
}

Test(ThenToken, init_then_token)
{
    char *str = "then";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, THEN);
}

Test(ElifToken, init_elif_token)
{
    char *str = "elif";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, ELIF);
}

Test(ElseToken, init_else_token)
{
    char *str = "else";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, ELSE);
}

Test(FiToken, init_fi_token)
{
    char *str = "fi";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, FI);
}

Test(SemiColonToken, init_semicolon_token)
{
    char *str = ";";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, SEMI_COLON);
}

Test(QuoteToken, init_quote_token)
{
    char *str = "\'";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, QUOTE);
}

Test(WordToken, init_word_token)
{
    char *str = "word";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, WORD);
}

Test(EchoToken, init_echo_token)
{
    char *str = "echo";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, ECHO);
}

Test(TrueToken, init_true_token)
{
    char *str = "true";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, T_TRUE);
}

Test(FalseToken, init_false_token)
{
    char *str = "false";
    struct lex_str *value = str_create(str);
    struct token *token = init_token(value);
    check_token(token, value, T_FALSE);
}

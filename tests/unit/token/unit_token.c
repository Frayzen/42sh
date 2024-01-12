#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

#include "tools/token/token.h"

void check_token(struct token *token, char *str, enum token_type type)
{
    cr_assert_neq(token, NULL, "token is null");
    cr_assert(strcmp(token->value, str) == 0, "incorrect value");
    cr_assert_eq(type, token->type, "incorrect type is %d expected %d",
                 token->type, type);
}

Test(NewlineToken, init_newline_token)
{
    char *str = "\n";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, NEWLINE);
    destroy_token(token);
}
Test(BSZeroToken, init_bszero_token)
{
    char *str = "\0";
    char *value = malloc(2);
    memcpy(value, str, 2);
    struct token *token = init_token(value);
    check_token(token, value, BSZERO);
    destroy_token(token);
}

Test(Token, init_if)
{
    char *str = "if";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, IF);
    destroy_token(token);
}

Test(ThenToken, init_then_token)
{
    char *str = "then";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, THEN);
}

Test(ElifToken, init_elif_token)
{
    char *str = "elif";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, ELIF);
    destroy_token(token);
}

Test(ElseToken, init_else_token)
{
    char *str = "else";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, ELSE);
    destroy_token(token);
}

Test(FiToken, init_fi_token)
{
    char *str = "fi";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, FI);
    destroy_token(token);
}

Test(SemiColonToken, init_semicolon_token)
{
    char *str = ";";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, SEMI_COLON);
    destroy_token(token);
}

Test(QuoteToken, init_quote_token)
{
    char *str = "\'";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, QUOTE);
}

Test(WordToken, init_word_token)
{
    char *str = "word";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, WORD);
}

Test(EchoToken, init_echo_token)
{
    char *str = "echo";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, ECHO);
}

Test(TrueToken, init_true_token)
{
    char *str = "true";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, T_TRUE);
}

Test(FalseToken, init_false_token)
{
    char *str = "false";
    char *value = malloc(strlen(str) + 1);
    strcpy(value, str);
    struct token *token = init_token(value);
    check_token(token, value, T_FALSE);
}

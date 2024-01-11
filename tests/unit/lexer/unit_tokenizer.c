#include "lexer/tokenizer.h"

#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>

TestSuite(tokenizer);

Test(TestSuite, test_colon)
{
    struct token *tok = next_token(";");
    cr_assert_eq(tok->type, SEMI_COLON);
    cr_assert_eq(tok->value, ";");
    cr_assert_eq(tok->terminal, true);
}

Test(TestSuite, test_if)
{
    struct token *tok = next_token("if");
    cr_assert_eq(tok->type, IF);
    cr_assert_eq(tok->value, "if");
    cr_assert_eq(tok->terminal, false);
}

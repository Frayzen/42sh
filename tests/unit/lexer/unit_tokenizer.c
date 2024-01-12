#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>

#include "io_backend/backend_saver.h"
#include "lexer/tokenizer.h"
#include "tools/token/token.h"

TestSuite(tokenizer);

Test(tokenizer, test_colon)
{
    io_push(";");
    struct token *tok = next_token();
    cr_assert_eq(tok->type, SEMI_COLON);
    cr_assert_str_eq(tok->value, ";", "Expected [%s] got [%s]", ";",
                     tok->value);
    cr_assert_eq(tok->terminal, true);
    destroy_token(tok);
}

Test(tokenizer, test_if)
{
    io_push("if");
    struct token *tok = next_token();
    cr_assert_eq(tok->type, IF);
    cr_assert_str_eq(tok->value, "if");
    cr_assert_eq(tok->terminal, false);
    destroy_token(tok);
}

#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <lexer/token_saver.h>

TestSuite(token_saver);

Test(token_saver, test_basic)
{
    cr_assert_eq(tok_peek()->type, IF); 
}

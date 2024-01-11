#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>

#include "io_backend/backend_saver.h"
#include "parser/grules/grules.h"
#include "tools/ast/ast.h"

TestSuite(easy_rules);

Test(easy_rules, test_word)
{
    io_push("WORD");
    struct ast *ast = grentry();
    cr_expect_not_null(ast);
    // TODO uncomment when lexer is good with io saver
    /* cr_expect_eq(ast->token->type, WORD); */
    /* cr_expect_eq(ast->token->value, "WORD"); */
    /* cr_expect_eq(ast->token->terminal, true); */
    cr_expect_null(ast->children);
    cr_expect_eq(ast->nb_children, 0);
    destroy_ast(ast);
}

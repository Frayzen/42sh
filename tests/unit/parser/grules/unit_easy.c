#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>

#include "io_backend/backend_saver.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

TestSuite(easy_rules);

Test(easy_rules, test_word)
{
    io_push("echo toto;");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_not_null(ast);
    cr_expect_str_eq(ast_to_str(ast), "CMD{echo,toto}");
    destroy_ast(ast);
}

Test(easy_rules, test_null_tree)
{
    io_push(";");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_null(ast);
    destroy_ast(ast);
}

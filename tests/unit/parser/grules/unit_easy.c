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
    // pretty_print_ast(ast);
    // TODO uncomment when lexer is good with io saver
    // cr_expect_eq(ast->type, AST_COMMAND);
    // cr_expect_eq(ast->nb_children, 2);
    // cr_expect_eq(ast->children[0]->type, AST_TOKEN);
    // cr_expect_eq(ast->children[1]->type, AST_TOKEN);
    // cr_expect_eq(ast->children[0]->token->type, ECHO);
    // cr_expect_eq(ast->children[1]->token->type, WORD);
    // cr_expect_str_eq(ast->children[0]->token->value, "echo");
    // cr_expect_str_eq(ast->children[1]->token->value, "toto");
    destroy_ast(ast);
}

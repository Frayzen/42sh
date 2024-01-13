#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <stdio.h>

#include "exit/exit.h"
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
    cr_expect_str_eq(ast_to_str(ast), "LST{CMD{echo,toto}}");
    destroy_ast(ast);
    clean(NULL);
}

Test(easy_rules, test_null_tree)
{
    io_push(";");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_null(ast);
    destroy_ast(ast);
    clean(NULL);
}

Test(easy_rule, list_double_echo)
{
    io_push("echo toto; echo tata");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_not_null(ast);
    cr_expect_str_eq(ast_to_str(ast), "LST{CMD{echo,toto},CMD{echo,tata}}");
    destroy_ast(ast);
    io_push("echo toto; echo tata;");
    struct ast *ast2 = NULL;
    cr_expect_eq(gr_input(&ast2), OK);
    cr_expect_not_null(ast2);
    cr_expect_str_eq(ast_to_str(ast2), "LST{CMD{echo,toto},CMD{echo,tata}}");
    destroy_ast(ast2);
    clean(NULL);
}

Test(easy_rules, long_list)
{
    io_push(
        "echo toto; echo tata; echo titi; echo foo; echo bar; echo baz; //    "
        "echo   biz    ; echo yipee yep; echo hello world !; echo 1 2 3 4 5 6");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_not_null(ast);
    cr_expect_str_eq(ast_to_str(ast),
                     "LST{CMD{echo,toto},CMD{echo,tata},CMD{echo,titi},CMD{"
                     "echo,foo},CMD{echo,bar},CMD{echo,baz},CMD{//"
                     ",echo,biz},CMD{echo,yipee,yep},CMD{echo,hello,world,!},"
                     "CMD{echo,1,2,3,4,5,6}}");
    destroy_ast(ast);
}

Test(conditions, simple_if)
{
    io_push("if true ; then echo kaka ; fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);

    cr_expect_str_eq(ast_to_str(ast),
                     "LST{IF{LST{CMD{true}},LST{CMD{echo,kaka}}}}");

    destroy_ast(ast);
}

Test(conditions, simple_if_else)
{
    io_push("if true ; then echo ; elif true; then echo; else echo ; fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_str_eq(ast_to_str(ast),
                     "LST{IF{LST{CMD{true}},LST{CMD{echo}},IF{LST{CMD{true}},"
                     "LST{CMD{echo}},LST{CMD{echo}}}}}");
    destroy_ast(ast);
}

Test(conditions, simple_if_elif_elif_else)
{
    io_push("if true ; then echo ; elif true; then echo; elif false; then "
            "true; else echo ; fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_str_eq(
        ast_to_str(ast),
        "LST{IF{LST{CMD{true}},LST{CMD{echo}},IF{LST{CMD{true}},LST{CMD{echo}},"
        "IF{LST{CMD{false}},LST{CMD{true}},LST{CMD{echo}}}}}}");
    destroy_ast(ast);
    clean(NULL);
}

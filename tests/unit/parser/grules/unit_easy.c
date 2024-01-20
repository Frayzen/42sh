#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <stdio.h>

#include "exit/error_handler.h"
#include "io_backend/backend_saver.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

void check_ast_str(struct ast *ast, const char *str)
{
    char *gen = ast_to_str(ast);
    cr_expect_str_eq(gen, str, "Expected %s but got %s", str, gen);
    clean(ast);
}

TestSuite(easy_rules);

Test(easy_rules, test_word)
{
    io_push("echo toto;");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_not_null(ast);
    check_ast_str(ast, "LST{PIPE{CMD{echo,toto}}}");
}

Test(easy_rules, test_null_tree)
{
    io_push(";");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_null(ast);
    check_ast_str(ast, "[NULL]");
}

Test(easy_rule, list_double_echo)
{
    io_push("echo toto; echo tata");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_not_null(ast);
    check_ast_str(ast, "LST{PIPE{CMD{echo,toto}},PIPE{CMD{echo,tata}}}");
}

Test(easy_rules, long_list)
{
    io_push(
        "echo toto; echo tata; echo titi; echo foo; echo bar; echo baz; //    "
        "echo   biz    ; echo yipee yep; echo hello world \\!; echo 1 2 3 4 5 "
        "6");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_not_null(ast);
    check_ast_str(
        ast,
        "LST{PIPE{CMD{echo,toto}},PIPE{CMD{echo,tata}},PIPE{CMD{echo,titi}},"
        "PIPE{CMD{echo,foo}},PIPE{CMD{echo,bar}},PIPE{CMD{echo,baz}},PIPE{CMD{/"
        "/,echo,biz}},PIPE{CMD{echo,yipee,yep}},PIPE{CMD{echo,hello,world,!}},"
        "PIPE{CMD{echo,1,2,3,4,5,6}}}");
}

Test(conditions, simple_if)
{
    io_push("if true ; then echo kaka ; fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);

    check_ast_str(
        ast, "LST{PIPE{IF{LST{PIPE{CMD{true}}},LST{PIPE{CMD{echo,kaka}}}}}}");
}

Test(conditions, simple_if_else)
{
    io_push("if true ; then echo ; elif true; then echo; else echo ; fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    check_ast_str(
        ast,
        "LST{PIPE{IF{LST{PIPE{CMD{true}}},LST{PIPE{CMD{echo}}},IF{LST{PIPE{CMD{"
        "true}}},LST{PIPE{CMD{echo}}},LST{PIPE{CMD{echo}}}}}}}");
}

Test(conditions, simple_if_elif_elif_else)
{
    io_push("if true ; then echo ; elif true; then echo; elif false; then "
            "true; else echo ; fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    check_ast_str(ast,
                  "LST{PIPE{IF{LST{PIPE{CMD{true}}},LST{PIPE{CMD{echo}}},IF{"
                  "LST{PIPE{CMD{true}}},LST{PIPE{CMD{echo}}},IF{LST{PIPE{CMD{"
                  "false}}},LST{PIPE{CMD{true}}},LST{PIPE{CMD{echo}}}}}}}}");
}

Test(conditions, compound_smpl_if1)
{
    io_push("if false; true; then \n echo a \n echo b; echo c; \n fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    check_ast_str(ast,
                  "LST{PIPE{IF{LST{PIPE{CMD{false}},PIPE{CMD{true}}},LST{PIPE{"
                  "CMD{echo,a}},PIPE{CMD{echo,b}},PIPE{CMD{echo,c}}}}}}");
}

Test(conditions, compound_smpl_if2)
{
    io_push("if false\n true\n then\n echo a\n echo b ; echo c\n fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    check_ast_str(ast,
                  "LST{PIPE{IF{LST{PIPE{CMD{false}},PIPE{CMD{true}}},LST{PIPE{"
                  "CMD{echo,a}},PIPE{CMD{echo,b}},PIPE{CMD{echo,c}}}}}}");
}

Test(conditions, compound_smpl_if2_with_nwline)
{
    io_push("if false\n true\n then\n echo a\n echo b ; echo c\nfi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    check_ast_str(ast,
                  "LST{PIPE{IF{LST{PIPE{CMD{false}},PIPE{CMD{true}}},LST{PIPE{"
                  "CMD{echo,a}},PIPE{CMD{echo,b}},PIPE{CMD{echo,c}}}}}}");
}

Test(conditions, compound_smpl_elif2)
{
    io_push("if false\n true\n then\n echo a\n echo b ; echo c\n elif "
            "true\nthen\n echo d\n else\n echo hello\n fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    check_ast_str(
        ast,
        "LST{PIPE{IF{LST{PIPE{CMD{false}},PIPE{CMD{true}}},LST{PIPE{CMD{echo,a}"
        "},PIPE{CMD{echo,b}},PIPE{CMD{echo,c}}},IF{LST{PIPE{CMD{true}}},LST{"
        "PIPE{CMD{echo,d}}},LST{PIPE{CMD{echo,hello}}}}}}}");
}

#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <stdio.h>

#include "exit/exit.h"
#include "io_backend/backend_saver.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

void check_ast_str(struct ast *ast, const char *str)
{
    char *gen = ast_to_str(ast);
    cr_expect_str_eq(gen, str);
    clean(ast);
}

TestSuite(easy_rules);

Test(easy_rules, test_word)
{
    io_push("echo toto;");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_not_null(ast);
    check_ast_str(ast, "LST{CMD{echo,toto}}");
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
    check_ast_str(ast, "LST{CMD{echo,toto},CMD{echo,tata}}");
}

Test(easy_rules, long_list)
{
    io_push(
        "echo toto; echo tata; echo titi; echo foo; echo bar; echo baz; //    "
        "echo   biz    ; echo yipee yep; echo hello world !; echo 1 2 3 4 5 6");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_not_null(ast);
    check_ast_str(ast,
                  "LST{CMD{echo,toto},CMD{echo,tata},CMD{echo,titi},CMD{"
                  "echo,foo},CMD{echo,bar},CMD{echo,baz},CMD{//"
                  ",echo,biz},CMD{echo,yipee,yep},CMD{echo,hello,world,!},"
                  "CMD{echo,1,2,3,4,5,6}}");
}

Test(conditions, simple_if)
{
    io_push("if true ; then echo kaka ; fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);

    check_ast_str(ast, "LST{IF{LST{CMD{true}},LST{CMD{echo,kaka}}}}");
}

Test(conditions, simple_if_else)
{
    io_push("if true ; then echo ; elif true; then echo; else echo ; fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    check_ast_str(ast,
                  "LST{IF{LST{CMD{true}},LST{CMD{echo}},IF{LST{CMD{true}},"
                  "LST{CMD{echo}},LST{CMD{echo}}}}}");
}

Test(conditions, simple_if_elif_elif_else)
{
    io_push("if true ; then echo ; elif true; then echo; elif false; then "
            "true; else echo ; fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    check_ast_str(
        ast,
        "LST{IF{LST{CMD{true}},LST{CMD{echo}},IF{LST{CMD{true}},LST{CMD{echo}},"
        "IF{LST{CMD{false}},LST{CMD{true}},LST{CMD{echo}}}}}}");
}

Test(conditions, compound_smpl_if1)
{
    io_push("if false; true; then \n echo a \n echo b; echo c; \n fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_str_eq(ast_to_str(ast),
                     "LST{IF{LST{CMD{false},CMD{true}},LST{CMD{echo,a},CMD{"
                     "echo,b},CMD{echo,c}}}}");
    destroy_ast(ast);
}

Test(conditions, compound_smpl_if2)
{
    io_push("if false\n true\n then\n echo a\n echo b ; echo c\n fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_str_eq(ast_to_str(ast),
                     "LST{IF{LST{CMD{false},CMD{true}},LST{CMD{echo,a},CMD{"
                     "echo,b},CMD{echo,c}}}}");
    destroy_ast(ast);
}

Test(conditions, compound_smpl_elif2)
{
    io_push("if false\n true\n then\n echo a\n echo b ; echo c\n elif true\n "
            "then\n echo d\n else\n echo hello\n fi");
    struct ast *ast = NULL;
    cr_expect_eq(gr_input(&ast), OK);
    cr_expect_str_eq(
        ast_to_str(ast),
        "LST{IF{LST{CMD{false},CMD{true}},LST{CMD{echo,a},CMD{echo,b},CMD{echo,"
        "c}},IF{LST{CMD{true}},LST{CMD{echo,d}},LST{CMD{echo,hello}}}}}");
    destroy_ast(ast);
}

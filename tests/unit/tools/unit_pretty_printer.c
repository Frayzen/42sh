
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include <stdio.h>

#include "io_backend/backend_saver.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}
Test(pretty_printer, basic_echo, .init = redirect_all_stdout)
{
    io_push("echo toto");
    struct ast *ast = NULL;
    cr_assert_eq(gr_input(&ast), OK);

    pretty_print_ast(ast);
    fflush(NULL);
    cr_assert_stdout_eq_str("CMD\n╠══echo\n╚══toto\n");
    destroy_ast(ast);
}

Test(pretty_printer, basic_echo_3_words, .init = redirect_all_stdout)
{
    io_push("echo toto tata");
    struct ast *ast = NULL;
    cr_assert_eq(gr_input(&ast), OK);

    pretty_print_ast(ast);
    fflush(NULL);
    cr_assert_stdout_eq_str("CMD\n╠══echo\n╠══toto\n╚══tata\n");
    destroy_ast(ast);
}

Test(pretty_printer, basic_lots_children, .init = redirect_all_stdout)
{
    io_push("echo toto tata titi tutu tutut the car is going crazy");
    struct ast *ast = NULL;
    cr_assert_eq(gr_input(&ast), OK);

    pretty_print_ast(ast);
    fflush(NULL);
    cr_assert_stdout_eq_str("CMD\n╠══echo\n╠══toto\n╠══tata\n╠══titi\n╠══tutu\n╠══tutut\n╠══the\n╠══car\n╠══is\n╠══going\n╚══crazy\n");
    destroy_ast(ast);
}

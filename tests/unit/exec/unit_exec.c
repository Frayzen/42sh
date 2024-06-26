#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>
#include <stdlib.h>

#include "exec/execs.h"
#include "exit/error_handler.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
static void redirect_all_stdout(void)
{
    cr_redirect_stdout();
}

Test(exec, one, .init = redirect_all_stdout)
{
    char *a[3] = { "blb", "-c", "echo toto" };
    main_to_stream(3, a);
    struct ast *e = NULL;
    gr_input(&e);
    cr_assert(e);
    exec_entry(e);
    cr_expect_stdout_eq_str("toto\n", "error");
    clean(e);
}

Test(exec, only_echo, .init = redirect_all_stdout)
{
    char *a[3] = { "blb", "-c", "echo" };
    main_to_stream(3, a);
    struct ast *e = NULL;
    gr_input(&e);
    cr_assert(e);
    exec_entry(e);
    cr_expect_stdout_eq_str("\n", "error");
    clean(e);
}

Test(exec, un_mot, .init = redirect_all_stdout)
{
    char *a[3] = { "blb", "-c", "echo if" };
    main_to_stream(3, a);
    struct ast *e = NULL;
    gr_input(&e);
    cr_assert(e);
    exec_entry(e);
    cr_expect_stdout_eq_str("if\n", "error");
    clean(e);
}

Test(exec, deux_mots, .init = redirect_all_stdout)
{
    char *a[3] = { "blb", "-c", "echo mot1 mot2" };
    main_to_stream(3, a);
    struct ast *e = NULL;
    gr_input(&e);
    cr_assert(e);
    exec_entry(e);
    cr_expect_stdout_eq_str("mot1 mot2\n", "error");
    clean(e);
}

Test(exec, trois_mots, .init = redirect_all_stdout)
{
    char *a[3] = { "blb", "-c", "echo mot1 mot2 mot3" };
    main_to_stream(3, a);
    struct ast *e = NULL;
    gr_input(&e);
    cr_assert(e);
    exec_entry(e);
    cr_expect_stdout_eq_str("mot1 mot2 mot3\n", "error");
    clean(e);
}

Test(exec, quatre_mots, .init = redirect_all_stdout)
{
    char *a[3] = { "blb", "-c", "echo mot1 mot2 mot3 mot4" };
    main_to_stream(3, a);
    struct ast *e = NULL;
    gr_input(&e);
    cr_assert(e);
    exec_entry(e);
    cr_expect_stdout_eq_str("mot1 mot2 mot3 mot4\n", "error");
    clean(e);
}

Test(exec, cinq_mots, .init = redirect_all_stdout)
{
    char *a[3] = { "blb", "-c", "echo mot1 mot2 mot3 mot4 mot5" };
    main_to_stream(3, a);
    struct ast *e = NULL;
    gr_input(&e);
    cr_assert(e);
    exec_entry(e);
    cr_expect_stdout_eq_str("mot1 mot2 mot3 mot4 mot5\n", "error");
    clean(e);
}

Test(exec, big_fat_sentence_with_if, .init = redirect_all_stdout)
{
    char *a[3] = { "blb", "-c", "echo if else then ok ca roule ma poule" };
    main_to_stream(3, a);
    struct ast *e = NULL;
    gr_input(&e);
    cr_assert(e);
    exec_entry(e);
    cr_expect_stdout_eq_str("if else then ok ca roule ma poule\n", "error");
    clean(e);
}

#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

#include "io_backend/backend_saver.h"
#include "lexer/finder.h"
#include "exec/exec_echo.h"
#include "io_backend/backend_saver.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
#include <stdlib.h>
static void redirect_all_stdout(void)
{
        cr_redirect_stdout();
}

Test(exec, one, .init=redirect_all_stdout)
{
  char *a[3] = {"blb", "-c", "echo toto"};
  main_to_stream(3, a);
  struct ast *e = NULL;
  gr_input(&e);
  cr_assert(e);
  echo_function(e);
  cr_expect_stdout_eq_str("toto\n", "error");
}

Test(exec, only_echo, .init=redirect_all_stdout)
{
  char *a[3] = {"blb", "-c", "echo"};
  main_to_stream(3, a);
  struct ast *e = NULL;
  gr_input(&e);
  cr_assert(e);
  echo_function(e);
  cr_expect_stdout_eq_str("", "error");
}
Test(exec, big_fat_sentence_with_if , .init=redirect_all_stdout)
{
  char *a[3] = {"blb", "-c", "echo if else then ok ca roule ma poule"};
  main_to_stream(3, a);
  struct ast *e = NULL;
  gr_input(&e);
  cr_assert(e);
  echo_function(e);
  cr_expect_stdout_eq_str("", "error");
}

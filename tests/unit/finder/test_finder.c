#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

#include "../../../src/finder/finder.h"
#include "../../../src/io_backend/backend_saver.h"
void redirect_all_stdout(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

Test(comments, at_end)
{
    io_put("if test");
    cr_assert_str_eq(finder(), "if");
    cr_assert_str_eq(finder(), "test");
}

Test(comments, space)
{
    io_put("   if   echotest");
    cr_assert_str_eq(finder(), "if");
    cr_assert_str_eq(finder(), "echotest");
}

Test(comments, space2)
{
    io_put("   if   echo test");
    cr_assert_str_eq(finder(), "if");
    cr_assert_str_eq(finder(), "echo");
    cr_assert_str_eq(finder(), "test");
}

Test(comments, semicolon)
{
    io_put("   if ;  echo; test;");
    cr_assert_str_eq(finder(), "if");
    cr_assert_str_eq(finder(), ";");
    cr_assert_str_eq(finder(), "echo");
    cr_assert_str_eq(finder(), ";");
    cr_assert_str_eq(finder(), "test");
    cr_assert_str_eq(finder(), ";");
}

Test(comments, backslashn)
{
    io_put("   if ;  echo\n; test;");
    cr_assert_str_eq(finder(), "if");
    cr_assert_str_eq(finder(), ";");
    cr_assert_str_eq(finder(), "echo");
    cr_assert_str_eq(finder(), "\n");
    cr_assert_str_eq(finder(), ";");
    cr_assert_str_eq(finder(), "test");
    cr_assert_str_eq(finder(), ";");
}

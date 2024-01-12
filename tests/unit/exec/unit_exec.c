#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

#include "io_backend/backend_saver.h"
#include "lexer/finder.h"

Test(comments, at_end)
{
    io_push("if test");
    cr_assert_str_eq(finder(), "if");
    cr_assert_str_eq(finder(), "test");
}

Test(comments, at_end2)
{
    io_push("hello test");
    cr_assert_str_eq(finder(), "hello");
    cr_assert_str_eq(finder(), "test");
}


Test(comments, space)
{
    io_push("   if   echotest");
    cr_assert_str_eq(finder(), "if");
    cr_assert_str_eq(finder(), "echotest");
}

Test(comments, space2)
{
    io_push("   if   echo test");
    cr_assert_str_eq(finder(), "if");
    cr_assert_str_eq(finder(), "echo");
    cr_assert_str_eq(finder(), "test");
}

Test(comments, semicolon)
{
    io_push("   if ;  echo; test;");
    cr_assert_str_eq(finder(), "if");
    cr_assert_str_eq(finder(), ";");
    cr_assert_str_eq(finder(), "echo");
    cr_assert_str_eq(finder(), ";");
    cr_assert_str_eq(finder(), "test");
    cr_assert_str_eq(finder(), ";");
}

Test(comments, backslashn)
{
    io_push("   if ;  echo\n; test;");
    cr_assert_str_eq(finder(), "if");
    cr_assert_str_eq(finder(), ";");
    cr_assert_str_eq(finder(), "echo");
    cr_assert_str_eq(finder(), "\n");
    cr_assert_str_eq(finder(), ";");
    cr_assert_str_eq(finder(), "test");
    cr_assert_str_eq(finder(), ";");
}

#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

#include "exit/error_handler.h"
#include "io_backend/backend_saver.h"
#include "lexer/finder.h"

void assert_and_clear(const char *val)
{
    const struct pending *pending = finder();
    struct lex_str str = pending->str;
    cr_assert_str_eq(str.value, val);
    free(str.value);
}

char *val = NULL;

Test(comments, at_end)
{
    io_push("if test");
    assert_and_clear("if");
    assert_and_clear("test");
    assert_and_clear("\0");
    clean(NULL);
}

Test(comments, space)
{
    io_push("   if   echotest");
    assert_and_clear("if");
    assert_and_clear("echotest");
    assert_and_clear("\0");
    clean(NULL);
}

Test(comments, space2)
{
    io_push("   if   echo test");
    assert_and_clear("if");
    assert_and_clear("echo");
    assert_and_clear("test");
    assert_and_clear("\0");
    clean(NULL);
}

Test(comments, semicolon)
{
    io_push("   if ;  echo; test;");
    assert_and_clear("if");
    assert_and_clear(";");
    assert_and_clear("echo");
    assert_and_clear(";");
    assert_and_clear("test");
    assert_and_clear(";");
    clean(NULL);
}

Test(comments, backslashn)
{
    io_push("   if ;  echo\n; test;");
    assert_and_clear("if");
    assert_and_clear(";");
    assert_and_clear("echo");
    assert_and_clear("\n");
    assert_and_clear(";");
    assert_and_clear("test");
    assert_and_clear(";");
    assert_and_clear("\0");
    clean(NULL);
}

Test(comments, quotes)
{
    io_push("   if ';  echo'\n; test;");
    assert_and_clear("if");
    assert_and_clear(";  echo");
    assert_and_clear("\n");
    assert_and_clear(";");
    assert_and_clear("test");
    assert_and_clear(";");
    assert_and_clear("\0");
    clean(NULL);
}

Test(comments, quotes_empyt)
{
    io_push("if ''  \n");
    assert_and_clear("if");
    assert_and_clear("");
    assert_and_clear("\n");
    assert_and_clear("\0");
    clean(NULL);
}

Test(comments, backslashn_fi)
{
    io_push("\nfi");
    assert_and_clear("\n");
    assert_and_clear("fi");
    assert_and_clear("\0");
    clean(NULL);
}

Test(comments, comments_chelou)
{
    io_push("echo not#ok");
    assert_and_clear("echo");
    assert_and_clear("not#ok");
    assert_and_clear("\0");
    clean(NULL);
}

Test(comments, comments_chelou2)
{
    io_push("echo not #ok");
    assert_and_clear("echo");
    assert_and_clear("not");
    assert_and_clear("\0");
    clean(NULL);
}
Test(comments, fi_no_space)
{
    io_push(";fi");
    assert_and_clear(";");
    assert_and_clear("fi");
    assert_and_clear("\0");
    clean(NULL);
}

#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>

#include "io_backend/backend_saver.h"
TestSuite(backend_saver);

Test(backend_saver, basic_put)
{
    io_push("tests");
    cr_assert_eq(io_peek(), 't');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_peek(), 'e');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_peek(), 's');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_peek(), 't');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_peek(), 's');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_pop(), false);
}

Test(backend_saver, basic_alphabet)
{
    cr_assert_eq(io_pop(), false);
    io_push("a");
    io_push("b");
    io_push("c");
    io_push("d");
    io_push("e");
    cr_assert_eq(io_peek(), 'a', "Expected a got %c", io_peek());
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_peek(), 'b');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_peek(), 'c');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_peek(), 'd');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_peek(), 'e');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_pop(), false);
}

Test(backend_saver, basic_long)
{
    cr_assert_eq(io_pop(), false);
    char cur[2] = { 0 };
    for (int i = 0; i < BACKEND_BUFFER_SIZE; i++)
    {
        cur[0] = 'a';
        cur[0] += i % 26;
        io_push(cur);
    }
    for (int i = 0; i < BACKEND_BUFFER_SIZE; i++)
    {
        char val = 'a';
        val += i % 26;
        cr_assert_eq(io_peek(), val);
        cr_assert_eq(io_pop(), true);
    }
}

Test(backend_saver, null_check)
{
    cr_assert_eq(io_pop(), false);
    io_put_chars("\0", 1);
    cr_assert_eq(io_peek(), '\0');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_pop(), false);
}

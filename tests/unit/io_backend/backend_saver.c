#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include "io_backend/backend_saver.h"

TestSuite(backend_saver);

Test(backend_saver, basic_put)
{
    io_put("tests");
    cr_assert_eq(io_get_char(), 't');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_get_char(), 'e');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_get_char(), 's');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_get_char(), 't');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_get_char(), 's');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_pop(), false);
}

Test(backend_saver, basic_alphabet)
{
    cr_assert_eq(io_pop(), false);
    io_put("a");
    io_put("b");
    io_put("c");
    io_put("d");
    io_put("e");
    cr_assert_eq(io_get_char(), 'a', "Expected a got %c", io_get_char());
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_get_char(), 'b');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_get_char(), 'c');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_get_char(), 'd');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_get_char(), 'e');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_pop(), false);
}


Test(backend_saver, basic_long)
{
    cr_assert_eq(io_pop(), false);
    char cur[2] = { 0 };
    for (int i = 0; i < RINGBUFSIZE; i++) {
        cur[0] = 'a';
        cur[0] += i % 26;
        io_put(cur);
    }
    for (int i = 0; i < RINGBUFSIZE; i++) {
        char val = 'a';
        val += i % 26;
        cr_assert_eq(io_get_char(), val);
        cr_assert_eq(io_pop(), true);
    }
}

Test(backend_saver, null_check)
{
    cr_assert_eq(io_pop(), false);
    io_put_chars("\0", 1);
    cr_assert_eq(io_get_char(), '\0');
    cr_assert_eq(io_pop(), true);
    cr_assert_eq(io_pop(), false);
}

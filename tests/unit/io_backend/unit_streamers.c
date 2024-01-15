#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

#include "exit/exit.h"
#include "io_backend/backend_saver.h"

#define TEST_STR "Hello World !"

extern void io_streamer_file(char *path);
extern void io_streamer_stdin(char *path);
extern void io_streamer_string(int argc, char **argv);

Test(streamers, file)
{
    char *file_path = "./tests/unit/io_backend/file_test_streamers";
    io_streamer_file(file_path);

    // -1 because file is \n terminated whereas TEST_STR is null terminated
    for (unsigned int i = 0; i < sizeof(TEST_STR) - 1; i++)
    {
        char c = io_peek();
        cr_assert_eq(io_peek(), TEST_STR[i], "Expected %c got %c", TEST_STR[i],
                     c);
        io_pop();
    }
    clean(NULL);
}

Test(streamers, wrong_file_path)
{
    char *wrong_file_path = "./wrong_file_path";
    io_streamer_file(wrong_file_path);
    cr_assert_eq(io_peek(), '\0');
    clean(NULL);
}

Test(streamers, string_good)
{
    int argc = 3;
    char *argv[3];
    argv[0] = "42sh";
    argv[1] = "-c";
    argv[2] = TEST_STR;
    io_streamer_string(argc, argv);
    for (unsigned int i = 0; i < sizeof(TEST_STR); i++)
    {
        char c = io_peek();
        cr_assert_eq(c, TEST_STR[i], "Expected %c got %c", TEST_STR[i], c);
        io_pop();
    }
    clean(NULL);
}

Test(streamers, string_null)
{
    int argc = 2;
    char *argv[2];
    argv[0] = "42sh";
    argv[1] = "-c";
    io_streamer_string(argc, argv);
    cr_assert_eq(io_peek(), '\0');
    clean(NULL);
}

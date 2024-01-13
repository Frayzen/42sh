#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

#include "exit/exit.h"
#include "io_backend/backend_saver.h"
#include "io_backend/io_streamers.h"

#define TEST_STR "Hello World !"

Test(streamers, file)
{
    char *file_path = "./tests/unit/io_backend/file_test_streamers";
    io_streamer_file(file_path);

    for (unsigned int i = 0; i < sizeof(TEST_STR); i++)
    {
        cr_assert_eq(io_peek(), TEST_STR[i]);
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
    argv[0] = "-c";
    argv[1] = TEST_STR, argv[2] = NULL;
    io_streamer_string(argc, argv);
    for (unsigned int i = 0; i < sizeof(TEST_STR); i++)
    {
        cr_assert_eq(io_peek(), TEST_STR[i]);
        io_pop();
    }
    clean(NULL);
}

Test(streamers, string_null)
{
    int argc = 3;
    char *argv[3];
    argv[0] = "Hello World !";
    argv[1] = "-c";
    argv[2] = NULL;
    io_streamer_string(argc, argv);
    cr_assert_eq(io_peek(), '\0');
    clean(NULL);
}

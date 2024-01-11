#include "io_backend/io_streamers.h"

#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <criterion/internal/test.h>
#include <criterion/redirect.h>

Test(streamers, file)
{
    char *file_path = "./tests/unit/io_backend/file_test_streamers";
    char *str = io_streamer_file(file_path);
    cr_assert(str);
    cr_assert_str_eq("Hello World !", str);
}

Test(streamers, wrong_file_path)
{
    char *wrong_file_path = "./wrong_file_path";
    char *str = io_streamer_file(wrong_file_path);
    cr_assert(!str);
}

Test(streamers, string_good)
{
    int argc = 2;
    char *argv[2];
    argv[0] = "-c";
    argv[1] = "Hello World !";
    char *str = io_streamer_string(argc, argv);
    cr_assert(str);
    cr_assert_str_eq(str, argv[1]);
}

Test(streamers, string_null)
{
    int argc = 2;
    char *argv[2];
    argv[0] = "Hello World !";
    argv[1] = "-c";
    char *str = io_streamer_string(argc, argv);
    cr_assert(!str);
}

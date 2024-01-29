#include "arg_parser.h"

#include <string.h>

#include "arg_saver/arg_saver.h"
#include "env/env.h"
#include "exit/error_handler.h"
#include "io_backend/io_streamers.h"
int set_options(int argc, char **argv)
{
    int i = 1;
    while (i < argc)
    {
        if (!strcmp(argv[i], "--pretty-print") || !strcmp(argv[i], "-p"))
            get_env_flag()->pretty_print = true;
        else if (!strcmp(argv[i], "--verbose") || !strcmp(argv[i], "-v"))
            get_env_flag()->verbose = true;
        else if (!strcmp(argv[i], "--debug-pipe") || !strcmp(argv[i], "-d"))
            get_env_flag()->debug_pipe = true;
        else if (!strcmp(argv[i], "--debug-env") || !strcmp(argv[i], "-e"))
            get_env_flag()->debug_env = true;
        else if (!strcmp(argv[i], "--debug-grammar") || !strcmp(argv[i], "-g"))
            get_env_flag()->debug_grammar = true;
        else
            break;
        i++;
    }

    return i;
}

void parse_args(int argc, char **argv)
{
    int i = set_options(argc, argv);

    argc -= i;
    argv += i - 1;
    if (argc == 0)
    {
        io_streamer_stdin();
    }
    else if (!strcmp(argv[1], "-c"))
    {
        if (1 >= argc)
            exit_gracefully(ARG_ERROR);
        io_streamer_string(argc, argv + 1);
    }
    else
    {
        new_arg_info(argc - 1, argv + 2);
        io_streamer_file(argv[1]);
    }
}

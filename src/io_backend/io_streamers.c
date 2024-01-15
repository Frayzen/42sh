#define _POSIX_C_SOURCE 200809L
#include "io_streamers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "env/env.h"
#include "exit/exit.h"
#include "io_backend/backend_saver.h"

#define IO_FILE (set_fd(NULL))

FILE *set_fd(FILE *new_file)
{
    static FILE *file = NULL;
    if (new_file == NULL)
        return file;
    file = new_file;
    return file;
}

void io_streamer_file(char *path_to_file)
{
    FILE *file = fopen(path_to_file, "r");
    if (!file)
    {
        print_error(FILE_COULDNT_OPEN);
        return;
    }
    fseek(file, 0, SEEK_SET);
    set_fd(file);
}

void io_streamer_string(int argc, char **argv)
{
    for (int i = 0; i < argc - 1; i++)
    {
        if (!strcmp(argv[i], "-c"))
        {
            char *buf = argv[i + 1];
            FILE *file = fmemopen(buf, strlen(buf), "r");
            set_fd(file);
        }
    }
    return;
}

void io_streamer_stdin(void)
{
    set_fd(stdin);
}

void main_to_stream(int argc, char **argv)
{
    int i = 1;
    while (1)
    {
        if (!strcmp(argv[i], "--pretty-print"))
            get_env_flag()->print = true;
        else if (!strcmp(argv[i], "--verbose"))
            get_env_flag()->verbose = true;
        else
            break;
        i++;
    }
    argc -= i;
    argv += i;
    if (argc == 0)
        io_streamer_stdin();
    else if (argc == 1)
        io_streamer_file(*argv);
    else if (argc == 2)
        io_streamer_string(argc, argv);
    else
        print_error(ARG_ERROR);
}

size_t stream_input(size_t size)
{
    if (IO_FILE == NULL)
        return 0;
    char *buffer = calloc(size, sizeof(char));
    int amount = getline(&buffer, &size, IO_FILE);
    io_push(buffer);
    free(buffer);
    return amount;
}

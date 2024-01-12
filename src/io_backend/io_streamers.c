#define _POSIX_C_SOURCE 200809L
#include "io_streamers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <stdio.h>
#include "io_backend/backend_saver.h"


void main_to_stream(int argc, char **argv)
{
    if (argc == 1)
        io_streamer_stdin();
    else if (argc == 2)
        io_streamer_file(argv[1]);
    else if (argc == 3)
        io_streamer_string(argc, argv);
    else
        //TODO error handling
        return;
}

void io_streamer_file(char *path_to_file)
{
    FILE *file = fopen(path_to_file, "r");
    if (!file)
    {
        // TODO Error handling
        return;
    }
    fseek(file, 0, SEEK_END);
    long length_of_file = ftell(file);
    char *buffer = malloc(length_of_file - 1);
    if (!buffer)
    {
        // TODO Error handling
        return;
    }
    fseek(file, 0, SEEK_SET);
    if (fread(buffer, 1, length_of_file, file) == 0)
        return;
    fclose(file);
    buffer[length_of_file - 1] = '\0';
    io_push(buffer);
}

void io_streamer_string(int argc, char **argv)
{
    for (int i = 0; i < argc - 1; i++)
    {
        if (!strcmp(argv[i], "-c"))
        {
            io_push(argv[i + 1]);
        }
    }
    return;
}

void io_streamer_stdin(void)
{
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, stdin) == -1)
    {
        // ERROR HANDLE
        return;
    }
    io_push(line);
}

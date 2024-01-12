#include "io_streamers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *io_streamer_file(char *path_to_file)
{
    FILE *file = fopen(path_to_file, "r");
    if (!file)
    {
        // TODO Error handling
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length_of_file = ftell(file);
    char *buffer = malloc(length_of_file - 1);
    if (!buffer)
    {
        // TODO Error handling
        return NULL;
    }
    fseek(file, 0, SEEK_SET);
    if (!fread(buffer, 2, length_of_file, file))
    {
        // TODO Error handling
        return NULL;
    }
    fclose(file);
    buffer[length_of_file - 1] = '\0';
    return buffer;
}

char *io_streamer_string(int argc, char **argv)
{
    for (int i = 0; i < argc - 1; i++)
    {
        if (!strcmp(argv[i], "-c"))
        {
            return argv[i + 1];
        }
    }
    // TODO Error handling
    return NULL;
}

char *io_streamer_stdin(void)
{
    char *line = NULL;
    size_t len = 0;
    if (getline(&line, &len, stdin) == -1)
    {
        // TODO Error handling
        return NULL;
    }
    return line;
}

#include "io_streamers.h"

#include <stdlib.h>
#include <stdio.h>
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
    fread(buffer, 1, length_of_file, file);
    fclose(file);
    buffer[length_of_file - 1] = '\0';
    return buffer;
}

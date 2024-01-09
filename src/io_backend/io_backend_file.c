#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *io_backend_file(char *path_to_file)
{
    int length = 0;
    FILE *file = fopen(path_to_file, "r");
    if (!file)
    {
        // TODO Error handling
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long length_of_file = ftell(file);
    char *buffer = malloc(length_of_file + 1);
    if (!buffer)
    {
        // TODO Error handling
        return NULL;
    }
    fread(buffer, 1, length_of_file, file);
    fclose(file);
    buffer[length_of_file + 1] = '\0';
    return buffer;
}

char *io_backend_string(int argc, char **argv)
{
    for (int i = 0; i < argc; i++)
    {
        if (!strcmp(argv[i], "-c"))   
        {
            return argv[i + 1];
        }
    }
    //TODO Error handling
    return NULL;
}

char *io_backend_stdin(void)
{
    
}

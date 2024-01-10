#include "io_streamers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *io_streamer_string(int argc, char **argv)
{
    for (int i = 0; i < argc - 1; i++)
    {
        if (!strcmp(argv[i], "-c"))
        {
            return argv[i + 1];
        }
    }
    return NULL;
}

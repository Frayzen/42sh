#include "io_streamers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *io_streamer_stdin(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t line_size = 0;
    line_size = getline(&line, &len, stdin);
    return line;
}

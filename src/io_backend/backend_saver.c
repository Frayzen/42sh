#include "io_backend/backend_saver.h"

#include "io_streamers.h"

static char next = '\0';

char io_peek(void)
{
    if (next == '\0')
        next = stream_next();
    return next;
}

void io_pop(void)
{
    next = '\0';
    return;
}

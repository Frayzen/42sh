#include <stdio.h>

#include "io_backend/backend_saver.h"
extern struct ringbuffer *get_buffer(void);
int main(void)
{
    io_push("TEST");
    printf("start %p\n", (void *)get_buffer()->begin);
    printf("end %p\n", (void *)get_buffer()->end);
    io_push("TEST");
    return 0;
}

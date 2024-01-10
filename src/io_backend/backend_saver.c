#include "io_backend/backend_saver.h"
#include <string.h>

struct ringbuffer *get_buffer(void)
{
    static struct ringbuffer rb = { 0 };
    if (rb.begin == NULL)
        rb.begin = rb.value;
    if (rb.end == NULL)
        rb.end = rb.value;
    return &rb;
}

size_t get_bufsize(void)
{
    struct ringbuffer *rb = get_buffer();
    if(rb->end < rb->begin)
        return RINGBUFSIZE - (rb->begin - rb->end);
    return rb->end - rb->begin;
}

void io_put(char *str)
{
    struct ringbuffer *rb = get_buffer();
    size_t l = strlen(str);
    size_t available = RINGBUFSIZE - get_bufsize();
    if(available < l)
    {
        l = available;
        //TODO error handling
    }
    size_t toend = rb->value + RINGBUFSIZE - rb->end;
    if (toend < l)
    {
        memcpy(rb->end, str, toend);
        str += toend;
        l -= toend;
        rb->end = rb->value;
    }
    memcpy(rb->end, str, l);
}
char io_get_char(void)
{
    struct ringbuffer *rb = get_buffer();
    return *rb->begin;
}

char io_pop(void)
{
    struct ringbuffer *rb = get_buffer();
    char v = *rb->begin;
    rb->begin++;
    if (rb->value + RINGBUFSIZE == rb->begin)
        rb->begin = rb->value;
    return v;
}

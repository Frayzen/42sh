#include "io_backend/backend_saver.h"

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
    if (rb->end < rb->begin)
        return RINGBUFSIZE - (rb->begin - rb->end);
    return rb->end - rb->begin;
}

void io_put_chars(char *str, size_t len)
{
    struct ringbuffer *rb = get_buffer();
    size_t available = RINGBUFSIZE - get_bufsize();
    if (available < len)
    {
        len = available;
        // TODO error handling
    }
    size_t toend = rb->value + RINGBUFSIZE - rb->end;
    if (toend < len)
    {
        memcpy(rb->end, str, toend);
        str += toend;
        len -= toend;
        rb->end = rb->value;
    }
    memcpy(rb->end, str, len);
    rb->end += len;
}

void io_put(char *str)
{
    io_put_chars(str, strlen(str));
}

char io_get_char(void)
{
    struct ringbuffer *rb = get_buffer();
    return *(rb->begin);
}

bool io_pop(void)
{
    struct ringbuffer *rb = get_buffer();
    if (rb->begin == rb->end)
        return false;
    rb->begin++;
    if (rb->value + RINGBUFSIZE == rb->begin)
        rb->begin = rb->value;
    return true;
}

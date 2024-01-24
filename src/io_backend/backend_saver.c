#include "io_backend/backend_saver.h"

#include <stdio.h>

#include "io_streamers.h"
#include "tools/ring_buffer/ring_buffer.h"

#define GET_BUFFER swap_backend_buffer(NULL);

struct ringbuffer *swap_backend_buffer(struct ringbuffer *new)
{
    static struct ringbuffer *rb = NULL;
    struct ringbuffer *ret_rb = rb;
    if (new)
      rb = new;
    if (rb == NULL)
    {
        rb = rb_create(RB_CHAR, BACKEND_BUFFER_SIZE);
        ret_rb = rb;
    }
    return ret_rb;
}

void io_push_chars(char *str, size_t len)
{
    struct ringbuffer *rb = GET_BUFFER;
    union ringitem item;
    for (size_t i = 0; i < len; i++)
    {
        item.c = str[i];
        rb_push(rb, item);
    }
}

void io_push(char *str)
{
    if (!str)
        return;
    io_push_chars(str, strlen(str));
}

char io_peek(void)
{
    struct ringbuffer *rb = GET_BUFFER;
    if (rb->cur_size == 0)
        stream_input(GET_BUFFER->ring_size);
    union ringitem *item = rb_peek(rb);
    if (!item)
        return '\0';
    return item->c;
}

bool io_pop(void)
{
    return rb_pop(GET_BUFFER);
}

void clean_backend_saver(void)
{
    rb_destroy(GET_BUFFER);
}

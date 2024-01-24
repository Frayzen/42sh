#include "io_backend/backend_saver.h"

#include <stdio.h>

#include "io_streamers.h"
#include "tools/definitions.h"
#include "tools/ring_buffer/ring_buffer.h"

static struct ringbuffer *get_buffer(void)
{
    static struct ringbuffer *rb = NULL;
    if (rb == NULL)
        rb = rb_create(RB_CHAR, BACKEND_BUFFER_SIZE);
    return rb;
}

void io_push_chars(char *str, size_t len)
{
    struct ringbuffer *rb = get_buffer();
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
    struct ringbuffer *rb = get_buffer();
    if (rb->cur_size == 0)
        stream_input(get_buffer()->ring_size);
    union ringitem *item = rb_peek(rb);
    if (!item)
        return '\0';
    return item->c;
}

BOOL io_pop(void)
{
    return rb_pop(get_buffer());
}

void clean_backend_saver(void)
{
    rb_destroy(get_buffer());
}

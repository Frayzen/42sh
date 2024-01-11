#include "io_backend/backend_saver.h"

#include "tools/ring_buffer/ring_buffer.h"

static struct ringbuffer *get_buffer(void)
{
    static struct ringbuffer *rb = NULL;
    if (rb == NULL)
        rb = rb_create(RB_CHAR, BACKEND_BUFFER_SIZE);
    return rb;
}

void io_put_chars(char *str, size_t len)
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
    io_put_chars(str, strlen(str));
}

char io_peek(void)
{
    struct ringbuffer *rb = get_buffer();
    return rb_peek(rb)->c;
}

bool io_pop(void)
{
    return rb_pop(get_buffer());
}

// TODO call this on program exit
void clean_bakend_saver(void)
{
    rb_destroy(get_buffer());
}

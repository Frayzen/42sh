#include <stddef.h>

#include "tools/ring_buffer/ring_buffer.h"
#include "lexer/tokenizer.h"

static struct ringbuffer *get_buffer(void)
{
    static struct ringbuffer *rb = NULL;
    if (rb == NULL)
        rb = rb_create(RB_TOKEN, 1);
    return rb;
}

void next(void)
{
    union ringitem next = {.token = next_token()};
    rb_push(get_buffer(), next);
}

struct token *tok_peek(void)
{
    struct ringbuffer *rb = get_buffer();
    union ringitem *res = rb_peek(rb);
    if (res != NULL)
        return res->token;
    next();
    res = rb_peek(rb);
    return res->token;
}

bool tok_pop(void)
{
    return rb_pop(get_buffer());
}

// TODO call this on program exit
void clean_token_saver(void)
{
    rb_destroy(get_buffer());
}

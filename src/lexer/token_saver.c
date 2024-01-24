#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer/tokenizer.h"
#include "tools/ring_buffer/ring_buffer.h"
#include "tools/token/token.h"


struct ringbuffer *swap_token_buffer(struct ringbuffer *new)
{
    static struct ringbuffer *rb = NULL;
    struct ringbuffer *ret_rb = rb;
    if (new)
      rb = new;
    if (rb == NULL)
    {
        rb = rb_create(RB_CHAR, 1);
        ret_rb = rb;
    }
    return ret_rb;
}

void next(void)
{
    union ringitem next = { .token = next_token() };
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

bool tok_pop_clean(void)
{
    struct token *tok = tok_peek();
    if (!rb_pop(get_buffer()))
        return false;
    destroy_token(tok);
    return true;
}

bool tok_pop(void)
{
    struct token *tok = tok_peek();
    int ret = rb_pop(get_buffer());
    free(tok);
    return ret;
}

void comsume_all(void)
{
    while (!tok_peek()->terminal)
        tok_pop_clean();
    tok_pop_clean();
}

void clean_token_saver(void)
{
    rb_destroy(get_buffer());
}

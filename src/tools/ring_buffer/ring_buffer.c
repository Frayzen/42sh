#include "ring_buffer.h"
#include <stdlib.h>
#include "tools/token/token.h"

void rb_push(struct ringbuffer *rb, union ringitem item)
{
    size_t available = rb->ring_size - rb->cur_size;
    if (available == 0)
    {
        // TODO error handling
        return;
    }
    *(rb->end) = item;
    rb->end++;
    rb->cur_size++;
}

union ringitem *rb_peek(struct ringbuffer *rb)
{
    if (rb->cur_size == 0)
        return NULL;
    return rb->begin;
}

bool rb_pop(struct ringbuffer *rb)
{
    if (rb->cur_size == 0)
        return false;
    rb->begin++;
    rb->cur_size--;
    if (rb->value + rb->ring_size == rb->begin)
        rb->begin = rb->value;
    return true;
}


void rb_destroy(struct ringbuffer *rb)
{
    union ringitem *item = NULL;
    switch (rb->type) {
        case RB_TOKEN:
        for (item = rb->begin; item != rb->end; item++)
            destroy_token(item->token);
        break;
            default:
       break; 
    }
    free(rb);
}
struct ringbuffer *rb_create(enum ringtype type, size_t ring_size)
{
    struct ringbuffer *rb = calloc(1, sizeof(struct ringbuffer));
    rb->ring_size = ring_size;
    rb->value = calloc(ring_size, sizeof(void *));
    rb->begin = rb->value;
    rb->end = rb->value;
    rb->type = type;
    return rb;
}

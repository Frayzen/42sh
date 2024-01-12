#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include "tools/token/token.h"
#include <stddef.h>

enum ringtype
{
    RB_UNKOWN = 0,
    RB_CHAR,
    RB_TOKEN,
};

union ringitem
{
    char c;
    struct token *token;
};

struct ringbuffer
{
    union ringitem *value;
    union ringitem *begin;
    union ringitem *end;
    size_t cur_size;
    size_t ring_size;
    enum ringtype type;
};

/***
 * Add item array to the rb buffer
 * @param i the item
 * @param len the number of union ringitem to add
 */
void rb_push(struct ringbuffer *rb, union ringitem item);

/***
 * Retrieve the first item from the buffer or NULL, do not change the buffer
 * @return The first item of the buffer or NULL if the buffer is empty
 * @see rb_pop() to access the next item
 */
union ringitem *rb_peek(struct ringbuffer *rb);

/***
 * Jump to the next item of the buffer
 * @return true if the jump happened, false if the buffer is empty
 * @see rb_peek() to get the current item
 */
bool rb_pop(struct ringbuffer *rb);

/***
 * Create a new ringbuffer
 * @param enum ringtype the type of the ringbuffer
 * @return the mallocated buffer
 */
struct ringbuffer *rb_create(enum ringtype type, size_t ring_size);

/***
 * Free a ringbuffer
 * @param rb the ringbuffer
 */
void rb_destroy(struct ringbuffer *rb);

#endif /* !RING_BUFFER_H */

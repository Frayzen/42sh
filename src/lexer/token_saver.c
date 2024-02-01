#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"
#include "tools/token/token.h"

static struct token *next = NULL;

struct token *swap_next_token(struct token *new)
{
    struct token *tok = next;
    next = new;
    return tok;
}

struct token *tok_peek(void)
{
    if (!next)
        next = next_token();
    return next;
}

void tok_pop_clean(void)
{
    if (!next)
        next = next_token();
    destroy_token(next);
    next = NULL;
}

void tok_pop(void)
{
    if (!next)
        next = next_token();
    free(next);
    next = NULL;
}

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"
#include "tools/token/token.h"

static struct token *next = NULL;
static struct token *next2 = NULL;

struct token *swap_next_token(struct token *new)
{
    struct token *tok = next;
    next = new;
    return tok;
}

struct token *swap_next_token2(struct token *new)
{
    struct token *tok = next2;
    next2 = new;
    return tok;
}

struct token *tok_peek(void)
{
    if (!next)
    {
        if (next2)
        {
            next = next2;
            next2 = NULL;
        }
        else
            next = next_token();
    }
    return next;
}

struct token *tok_peek2(void)
{
    tok_peek();
    next2 = next_token();
    return next2;
}

void tok_pop_clean(void)
{
    if (!next)
        next = next_token();
    destroy_token(next);
    next = next2;
    next2 = NULL;
}

void tok_pop(void)
{
    if (!next)
        next = next_token();
    free(next);
    next = next2;
    next2 = NULL;
}

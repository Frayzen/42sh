#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "tokenizer.h"
#include "tools/token/token.h"

static struct token *next = NULL;

struct token *tok_peek(void)
{
    if (!next)
        next = next_token();
    return next;
}

void tok_pop_clean(void)
{
    destroy_token(next);
    next = NULL;
}

void tok_pop(void)
{
    free(next);
    next = NULL;
}

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "env/env.h"
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

struct token *tok_peek(void)
{
    if (!next)
        next = next_token();
    return next;
}

struct token *tok_peek2(void)
{
    if (!next)
        return tok_peek();
    if (!next2)
        next2 = next_token();
    return next2;
}

void tok_pop_clean(void)
{
    if (next->type == BSZERO)
        get_env_flag()->null_received = true;
    if (!next)
        next = next_token();
    destroy_token(next);
    next = next2;
    next2 = NULL;
}

void tok_pop(void)
{
    if (next->type == BSZERO)
        get_env_flag()->null_received = true;
    if (!next)
        next = next_token();
    free(next);
    next = next2;
    next2 = NULL;
}

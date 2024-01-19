#include "tokenizer.h"

#include <stdlib.h>

#include "env/env.h"
#include "lexer/finder.h"
#include "tools/token/token.h"
struct token *next_token(void)
{
    const struct pending *pending = finder();
    if (!pending)
    {
        struct token *ret = calloc(1, sizeof(struct token));
        ret->type = TOK_ERROR;
        return ret;
    }
    struct token *result = init_token(&pending->str);
    if (pending->force_word)
    {
        result->terminal = false;
        result->type = WORD;
    }
    if (result->type == BSZERO)
        get_env_flag()->null_received = true;
    if (get_env_flag()->verbose)
        print_token(result);
    return result;
}

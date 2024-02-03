#define _POSIX_C_SOURCE 200809L
#include "tokenizer.h"

#include <stdlib.h>

#include "env/env.h"
#include "finder/finder.h"
#include "tools/definitions.h"
#include "tools/token/token.h"
struct token *next_token(void)
{
    struct pending *pending = finder();
    if (!pending)
    {
        struct token *ret = calloc(1, sizeof(struct token));
        ret->type = TOK_ERROR;
        return ret;
    }
    struct token *result = init_token(&pending->str);

    // check that pending quoted and is not assignment
    if (pending->force_str && result->type != ASSMT)
    {
        result->type = WORD;
        result->terminal = FALSE_B;
    }
    if (result->type == BSZERO)
        get_env_flag()->null_received = TRUE_B;
    if (get_env_flag()->verbose)
        print_token(result);
    return result;
}

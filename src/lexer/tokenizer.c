#include "tokenizer.h"

#include <stdio.h>

#include "env/env.h"
#include "lexer/finder.h"
struct token *next_token(void)
{
    struct token *result = init_token(finder());
    if (get_env_flag()->verbose)
        verbose(result);
    return result;
}

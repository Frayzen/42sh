#include "tokenizer.h"

#include "env/env.h"
#include "lexer/finder.h"
#include "tools/token/token.h"
struct token *next_token(void)
{
    const struct string *str = finder();
    struct token *result = init_token(str);
    if (result->type == BSZERO)
        get_env_flag()->null_received = true;
    if (get_env_flag()->verbose)
        verbose(result);
    return result;
}

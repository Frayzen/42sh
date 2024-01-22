#include "tokenizer.h"


#include "env/env.h"
#include "finder/finder.h"
#include "tools/token/token.h"
struct token *next_token(void)
{
    struct pending *pending = finder();
    struct token *result = init_token(&pending->str);
    if (result->type == BSZERO)
        get_env_flag()->null_received = true;
    if (get_env_flag()->verbose)
        print_token(result);
    return result;
}

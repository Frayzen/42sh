#include "tokenizer.h"
#include <stdlib.h>

#include "env/env.h"
#include "lexer/finder.h"
#include "tools/token/token.h"
struct token *next_token(void)
{
    const struct pending *pending = finder();
    print_string(&pending->str);
    const struct pending *pending2 = finder();
    print_string(&pending2->str);
    // const struct pending *pending3 = finder();
    // print_string(pending3->str);
    exit(0);
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

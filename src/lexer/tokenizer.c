#include "tokenizer.h"

#include <stdlib.h>

#include "env/env.h"
#include "lexer/finder.h"
#include "tools/str/string.h"
#include "tools/token/token.h"
struct token *next_token(void)
{
    struct pending *pending = finder();
    struct string *str = dup_str(&pending->str);
    struct token *result = init_token(str);
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

#include "tokenizer.h"
#include "lexer/finder.h"

struct token *next_token(void)
{
    struct token *result = init_token(finder());
    return result;
}

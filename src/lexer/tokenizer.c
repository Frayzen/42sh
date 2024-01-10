#include "tokenizer.h"

struct token *next_token(char *str)
{
    struct token *result = init_token(str);
    // TODO remove the str argument and get the lexeme from finder()
    return result;
}

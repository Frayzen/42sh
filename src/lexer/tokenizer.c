#include "tokenizer.h"
#include <stdio.h>

#include "lexer/finder.h"

struct token *next_token(void)
{
    char *got = finder();
    printf("GOT %s\n", got);
    struct token *result = init_token(got);
    return result;
}

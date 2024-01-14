#include "tokenizer.h"

#include <stdio.h>

#include "lexer/finder.h"
#include "tools/token/token.h"

struct token *next_token(void)
{
    const struct string *str = finder();
    /* printf("GOT %s\n", str->value); */
    struct token *result = init_token(str);
    return result;
}

#include "env.h"

#include <stdlib.h>
struct env_flag *get_env_flag(void)
{
    static struct env_flag flag = { .print = 0, .verbose = 0 };
    return &flag;
}

void verbose(struct token *token)
{
    size_t i = 0;
    while (TOK_TYPE_LT[i])
    {
        if (!strcmp(token->value, TOK_TYPE_LT[i]))
        {
            printf("%s: %s\n", TOK_TYPE_LT[i], token->value);
            break;
        }
        i++;
    }
}

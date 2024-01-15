#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct env_flag *get_env_flag(void)
{
    static struct env_flag flag = { .print = 0, .verbose = 0 };
    return &flag;
}

void verbose(struct token *token)
{
    size_t i = 0;
    int done = 0;
    while (TOK_TYPE_LT[i])
    {
        if (!strcmp(token->value, TOK_TYPE_LT[i]))
        {
            const char *type = TOK_TYPE_LT[i];
            if (!strcmp(type, "\n"))
                printf("%s: %s\n", "NEWLINE", "\\n");
            else if (!strcmp(type, "\0"))
                printf("%s: %s\n", "BSZERO", "\\0");

            else
                printf("%s: %s\n", type, token->value);
            done = 1;
            break;
        }
        i++;
    }
    if (!done)
        printf("%s: %s\n", "WORD", token->value);
}

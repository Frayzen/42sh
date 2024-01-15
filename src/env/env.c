#include "env.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools/token/token.h"
struct env_flag *get_env_flag(void)
{
    static struct env_flag flag = { .print = 0, .verbose = 0 };
    return &flag;
}

void verbose(struct token *token)
{
    size_t i = 0;
    const char **tok_type = toktype_lookup();
    int done = 0;
    while (tok_type[i])
    {
        if (!strcmp(token->value, tok_type[i]))
        {
            const char *type = tok_type[i];
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

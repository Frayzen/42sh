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
    const char **tok_type = toktype_lookup();
    const char *tok = tok_type[token->type];
    char *value = token->value;
    if (!tok)
        printf("%s: %s\n", "WORD", value);
    else if (!strcmp(tok, "\n"))
        printf("%s: %s\n", "NEWLINE", "\\n");
    else if (!strcmp(tok, "\0"))
    {
        printf("%s: %s\n", "BSZERO", "\\0");
    }
    else
        printf("%s: %s\n", tok, value);
}

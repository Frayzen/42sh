#include "env.h"

#include <stdbool.h>

#include "tools/token/token.h"
struct env *get_env_flag(void)
{
    static struct env flag = { .print = false,
                               .verbose = false,
                               .str_input = true };
    return &flag;
}

void verbose(struct token *token)
{
    print_token(token);
}

#include "env.h"

#include <stdbool.h>

struct env *get_env_flag(void)
{
    static struct env flag = { .pretty_print = false,
                               .verbose = false,
                               .debug_pipe = false,
                               .debug_grammar = false,
                               .null_received = false };
    return &flag;
}

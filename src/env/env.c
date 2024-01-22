#include "env.h"

#include <stdbool.h>

struct env *get_env_flag(void)
{
    static struct env flag = {
        .verbose = false,
        .is_interactive = false,
        .null_received = false,
        .pretty_print = false,
        .debug_pipe = false,
        .fds = { 0, 1, 2, -1 },
    };
    return &flag;
}

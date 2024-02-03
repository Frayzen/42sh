#include "env.h"

#include <stdint.h>

struct env *get_env_flag(void)
{
    static struct env flag = {
        .verbose = FALSE_B,
        .is_interactive = FALSE_B,
        .null_received = FALSE_B,
        .pretty_print = FALSE_B,
        .debug_env = FALSE_B,
        .debug_pipe = FALSE_B,
        .fds = { 0, 1, 2, -1 },
    };
    return &flag;
}

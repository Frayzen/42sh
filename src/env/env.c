#include "env.h"
#include <stdlib.h>
struct env_flag *get_env_flag(void)
{
    static struct env_flag flag = {.print = 0, .verbose = 0};
    return &flag;
}

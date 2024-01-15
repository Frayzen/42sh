#include "env.h"

struct env_flag *get_env_flag(void)
{
    static struct env_flag flag = calloc(1, sizeof(struct env_flag));
    flag.print = false;
    flag.verbose = false;
    return flag;
}

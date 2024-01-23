#include "vars.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env/env.h"

#define DBG_VAR(...)                                                           \
    if (get_env_flag()->debug_env)                                             \
        dprintf(DBG_OUT, __VA_ARGS__);

void setup_vars(void)
{
    assign_var("IFS", " \t\r");
    char path[PATH_MAX];
    assign_var("PWD", getcwd(path, PATH_MAX));
    assign_var("OLDPWD", path);
}

char *assign_var(char *name, char *value)
{
    char *old = getenv(name);
    DBG_VAR("assign |%s| to |%s|, old = |%s|\n", name, value, old);
    if (!name || !value)
        return old;
    setenv(name, value, 1);
    return old;
}

char *retrieve_var(char *name)
{
    DBG_VAR("attemp retrieving |%s|\n", name);
    if (!name)
        return NULL;
    char *value = getenv(name);
    DBG_VAR("|%s|=|%s|\n", name, value);
    return value;
}

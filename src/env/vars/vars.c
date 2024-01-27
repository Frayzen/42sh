#define _POSIX_C_SOURCE 200809L
#include "vars.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env/env.h"

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
    DBG_VAR("[VAR] assign |%s| to |%s|, old = |%s|\n", name, value, old);
    if (!name || !value)
        return old;
    setenv(name, value, 1);
    return old;
}

char *read_var(char *name)
{
    char *value = NULL;
    if (name)
        value = getenv(name);
    if (!value)
        value = "";
    return value;
}

char *retrieve_var(char *name)
{
    DBG_VAR("attemp retrieving |%s|\n", name);
    char *value = read_var(name);
    value = strdup(value);
    DBG_VAR("|%s|=|%s|\n", name, value);
    return value;
}

bool unset_var(char *name)
{
    bool exist = getenv(name) != NULL;
    DBG_VAR("[VAR] unset |%s|\n", name);
    if (unsetenv(name) || !exist)
        return false;
    return true;
}

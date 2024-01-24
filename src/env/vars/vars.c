#define _POSIX_C_SOURCE 200809L
#include "vars.h"

#include <stdio.h>
#include <stdlib.h>

#include "env.h"

char *assign(char *name, char *value)
{
    char *old = getenv(name);
    DBG_VAR("[VAR] assign |%s| to |%s|, old = |%s|\n", name, value, old);
    if (!name || !value)
        return old;
    setenv(name, value, 1);
    return old;
}

char *retrieve(char *name)
{
    DBG_VAR("attemp retrieving |%s|\n", name);
    if (!name)
        return NULL;
    char *value = getenv(name);
    DBG_VAR("[VAR] |%s|=|%s|\n", name, value);
    return value;
}

bool unset_var(char *name)
{
    bool exist = retrieve(name) != NULL;
    DBG_VAR("[VAR] unset |%s|\n", name);
    if (unsetenv(name) || !exist)
        return false;
    return true;
}

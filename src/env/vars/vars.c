#include "vars.h"

#include <stdlib.h>

char *assign(char *name, char *value)
{
    char *old = getenv(name);
    DBG_VAR("assign |%s| to |%s|, old = |%s|\n", name, value, old);
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
    DBG_VAR("|%s|=|%s|\n", name, value);
    return value;
}
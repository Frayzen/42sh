#define _POSIX_C_SOURCE 200809L
#include "vars.h"

#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env.h"
#include "env/env.h"
#include "var_dict.h"

void export_char(char *name)
{
    struct sh_var *var = get_var(name);
    var->exported = true;
    setenv(name, var->value, 1);
}

char *assign_var(char *name, char *value)
{
    struct sh_var *var = get_or_create_var(name);
    char *old = var->value;
    if (var->exported)
        setenv(name, value, 1);
    var->value = strdup(value);
    DBG_VAR("[VAR] assign |%s| to |%s|, old = |%s|\n", name, value, old);
    if (!name || !value)
        return old;
    return old;
}

char *read_var(char *name)
{
    struct sh_var *var = get_var(name);
    if (!var)
        return "";
    return var->value;
}

char *retrieve_var(char *name)
{
    DBG_VAR("attemp retrieving |%s|\n", name);
    char *value = read_var(name);
    value = strdup(value);
    DBG_VAR("|%s|=|%s|\n", name, value);
    return value;
}

BOOL unset_var(char *name)
{
    struct sh_var *var = get_var(name);
    if (!var)
        return false;
    if (var->exported)
        unsetenv(name);
    DBG_VAR("[VAR] unset |%s|\n", name);
    return remove_var(name);
}

static void init_env_var(char *name, char *val)
{
    struct sh_var *var = get_or_create_var(name);
    var->value = strdup(val);
    var->exported = true;
}

void init_env_vars(void)
{
    int i = 0;
    while (environ[i])
    {
        char *name = strdup(environ[i]);
        char *eq = strchr(environ[i], '=');
        char *val = "";
        if (eq)
        {
            val = eq + 1;
            *eq = '\0';
        }
        init_env_var(name, val);
        free(name);
        i++;
    }

    if (!get_var("IFS"))
        init_env_var("IFS", "\t\r ");
    char path[PATH_MAX];
    getcwd(path, PATH_MAX);
    if (!get_var("PWD"))
        init_env_var("PWD", path);
    if (!get_var("OLDPWD"))
        init_env_var("OLDPWD", path);
}

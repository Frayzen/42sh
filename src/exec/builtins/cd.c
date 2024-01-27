#define _POSIX_C_SOURCE 200809L
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "env/env.h"
#include "env/vars/vars.h"

int builtin_cd(char **argv)
{
    char *new = NULL;
    // If no arguments, go to the HOME folder
    if (!argv[1])
        new = read_var("HOME");
    else
    {
        // If the argument is -, go to the last folder (aka OLDPWD)
        if (!strcmp(argv[1], "-"))
        {
            new = read_var("OLDPWD");
            dprintf(STDOUT, "%s\n", new);
        }
        else
            // Otherwise, try to go to the argument
            new = argv[1];
    }
    if (chdir(new) == -1)
    {
        perror("Error on cd: ");
        return 1;
    }
    else
    {
        char path[PATH_MAX];
        // Set the old pwd to the old pwd value and set the pwd value to the
        // current return of getcwd
        assign_var("OLDPWD", assign_var("PWD", getcwd(path, PATH_MAX)));
    }
    return 0;
}

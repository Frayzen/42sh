#include <stddef.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "env/vars/vars.h"

void builtin_cd(char **argv)
{
    char *new = NULL;
    if (!argv[1])
        new = retrieve_var("HOME");
    else
    {
        if (!strcmp(argv[1], "-"))
            new = retrieve_var("OLDPWD");
        else
            new = argv[1];
    }
    chdir(new);
    assign_var("OLDPWD", assign_var("HOME", new));
}

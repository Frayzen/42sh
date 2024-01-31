#include <string.h>

#include "builtins.h"
#include "env/vars/vars.h"
#include "exit/error_handler.h"
#include "lexer/finder/finder_tools.h"

bool my_strchr(char *str, char chr, char **end)
{
    int i = 0;
    while (str[i] && str[i] != chr)
        i ++;
    *end = str + i;
    return str[i] == chr;
}

int builtin_export(char **argv)
{
    int i = 1;
    char *arg = NULL;
    char *eq = NULL;
    int ret = 0;
    while (argv[i])
    {
        arg = argv[i];
        i++;
        bool is_assignment = my_strchr(arg, '=', &eq);
        *eq = '\0';
        if (!is_name(arg, eq - arg))
        {
            print_error(BAD_VAR_NAME);
            ret = 1;
            continue;
        }
        if (is_assignment)
            assign_var(arg, eq + 1);
        export_var(arg);
    }
    return ret;
}

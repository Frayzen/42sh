#include "builtins.h"
#include "env/vars/vars.h"
#include "exit/error_handler.h"
#include "tools/funct_dict/funct_dict.h"

enum unset_mode
{
    UNDEFINED,
    UNSET_VAR,
    UNSET_FN,
};

// @return true if it is an option
#define OK_OPTION 1
#define OK_NOT_OPTION 2
#define ERROR 3
int set_option(char *arg, enum unset_mode *mode)
{
    if (arg[0] != '-')
        return OK_NOT_OPTION;
    int i = 1;
    while (arg[i])
    {
        char c = arg[i];
        if (c == 'v')
        {
            if (*mode == UNSET_FN)
            {
                print_error(UNSET_NOT_VAR_AND_FN);
                return 1;
            }
            *mode = UNSET_VAR;
        }
        else if (c == 'f')
        {
            if (*mode == UNSET_VAR)
            {
                print_error(UNSET_NOT_VAR_AND_FN);
                return 1;
            }
            *mode = UNSET_FN;
        }
        else
        {
            print_error(UNSET_NOT_VAR_AND_FN);
            return ERROR;
        }
        i++;
    }
    return OK_OPTION;
}

// return false on error, set i accordingly otherwise
int consume_options(char **argv, enum unset_mode *mode, int *i)
{
    while (argv[*i])
    {
        int ret = set_option(argv[*i], mode);
        if (ret == OK_NOT_OPTION)
            break;
        if (ret == ERROR)
            return false;
        (*i)++;
    }
    if (*mode == UNDEFINED)
        *mode = UNSET_VAR;
    return true;
}

int builtin_unset(char **argv)
{
    int i = 1;
    enum unset_mode mode = UNDEFINED;
    if (!argv[i])
        return 0;
    int err = consume_options(argv, &mode, &i);
    if (!err)
        return 2;
    while (argv[i])
    {
        if (mode == UNSET_VAR)
            unset_var(argv[i]);
        else
            funct_dict_pop(argv[i]);
        i++;
    }
    return 0;
}

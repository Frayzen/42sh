#include "builtins.h"
#include "env/vars/vars.h"
#include "exit/error_handler.h"

enum unset_mode
{
    UNDEFINED,
    UNSET_VAR,
    UNSET_FN,
};

int consume_options(char **argv, enum unset_mode *mode, int *i)
{
    while (argv[*i][0] == '-')
    {
        int j = 1;
        while (argv[*i][j])
        {
            char c = argv[*i][j];
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
                return 2;
            }
            j++;
        }
        i++;
    }
    if (*mode == UNDEFINED)
        *mode = UNSET_VAR;
    return 0;
}

int builtin_unset(char **argv)
{
    int i = 1;
    enum unset_mode mode = UNDEFINED;
    if (!argv[i])
        return 0;
    int err = consume_options(argv, &mode, &i);
    if (err)
        return err;
    while (argv[i])
    {
        if (mode == UNSET_VAR)
            unset_var(argv[i]);
        else
            // TODO unset function
            unset_var(argv[i]);
        i++;
    }
    return 0;
}

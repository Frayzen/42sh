#include <stdlib.h>

#include "builtins.h"
#include "exit/error_handler.h"

void builtin_exit(char **argv)
{
    clean(*AST_ROOT);
    if (!argv[1])
        exit(0);
    else
        exit(atoi(argv[1]));
}

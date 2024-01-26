#include <stdlib.h>

#include "builtins.h"
#include "exit/error_handler.h"

void builtin_exit(char **argv)
{
    clean(*AST_ROOT);
    exit(atoi(argv[1]));
}

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <unistd.h>

#include "exit.h"

void print_error(enum error_type type)
{
    static const char *const error_names[] = {
        [ARG_ERROR] = "The arguments did not match",
        [GRAMMAR_ERROR_ENTRY] = "The entry grammar failed to find a match",
        [EXECVP_FAILED] = "The provided binary doesn't exist",
        [RING_BF_FULL] = "Ring buffer is full",
        [MALLOC_NULL] = "Memory error : running out of RAM ?",
        [FILE_COULDNT_OPEN] = "The file couldnt be open",
        [ADD_CHILD_ERROR] = "Tried to add a child to an ast with NULL children",
        [FORK_ERROR] = "An error occured while forking another program",
    };
    dprintf(STDERR_FILENO, "%s\n", error_names[type]);
}

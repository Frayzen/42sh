#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "exit/exit.h"
#include "tools/ast/ast.h"

void exit_gracefully(enum error_type type)
{
    clean(AST_ROOT);
    print_error(type);
    if (type == ARG_ERROR || type == GRAMMAR_ERROR_ENTRY || type == FORK_ERROR)
        exit(2);
    else if (type == EXECVP_FAILED || type == INVALID_FILE_PATH)
        exit(127);
    else
        exit(1);
}

void print_error(enum error_type type)
{
    static const char *const error_names[] = {
        [ARG_ERROR] = "The arguments did not match",
        [GRAMMAR_ERROR_ENTRY] = "The entry grammar failed to find a match",
        [EXECVP_FAILED] = "The provided binary doesn't exist",
        [RING_BF_FULL] = "Ring buffer is full",
        [MALLOC_NULL] = "Memory error : running out of RAM ?",
        [ADD_CHILD_ERROR] = "Tried to add a child to an ast with NULL children",
        [FORK_ERROR] = "An error occured while forking another program",
        [LIST_NOT_FOUND] = "The element in the list ast is invalid",
        [INVALID_FILE_PATH] = "No such file or directory",
    };
    dprintf(STDERR_FILENO, "%s\n", error_names[type]);
}

#define _POSIX_C_SOURCE 200809L
#include "exit/error_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "env/env.h"
#include "tools/ast/ast.h"

// Do NOT expect this function to exit the program
void exit_gracefully(enum error_type type)
{
    print_error(type);
    if (get_env_flag()->is_interactive)
        return;
    struct ast *root = *AST_ROOT;
    if (root)
        clean(root);
    switch (type)
    {
    case ARG_ERROR:
    case GRAMMAR_ERROR_ENTRY:
    case FORK_ERROR:
    case UNEXPECTED_EOF:
        exit(2);
    case NO_EXEC_PERM:
    case NO_READ_PERM:
        exit(126);
    case EXECVP_FAILED:
    case INVALID_FILE_PATH:
        exit(127);
    default:
        exit(1);
    }
}

void print_error(enum error_type type)
{
    static const char *const error_names[] = {
        // General Errors -> 1
        [MALLOC_NULL] = "Memory error : running out of RAM ?",
        [UNSET_NOT_VAR_AND_FN] = "Cannot unset function AND variable at once",
        [RING_BF_FULL] = "Ring buffer is full",
        [ADD_CHILD_ERROR] = "Tried to add a child to an ast with NULL children",
        [LIST_NOT_FOUND] = "The element in the list ast is invalid",
        [PIPE_NOT_FOUND] = "The element in the pipe ast is invalid",
        [FD_DICO_FULL] = "The file descriptor dictionary is full",
        // Misuse of shell built-ins -> 2
        [ARG_ERROR] = "The arguments did not match",
        [GRAMMAR_ERROR_ENTRY] = "The entry grammar failed to find a match",
        [FORK_ERROR] = "An error occured while forking another program",
        //  Command cannot execute (permission or not an executable) -> 126
        [NO_EXEC_PERM] = "Permission to execute denied",
        [NO_READ_PERM] = "Permission to read denied",
        [INVALID_FILE_PATH] = "No such file or directory",
        // Command not found -> 127
        [EXECVP_FAILED] = "Command not found",
        [UNEXPECTED_EOF] = "Unexpected end of file",
        [BAD_REDIRECTION] = "Bad redirection",
        [BAD_FD] = "The file descriptor couldn't be open",
    };
    dprintf(STDERR_FILENO, "%s\n", error_names[type]);
}

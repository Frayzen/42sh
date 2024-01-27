#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include "tools/ast/ast.h"

enum error_type
{
    // General Errors -> 1
    MALLOC_NULL,
    RING_BF_FULL,
    ADD_CHILD_ERROR,
    LIST_NOT_FOUND,
    PIPE_NOT_FOUND,
    FD_DICO_FULL,
    UNEXPECTED_EOF,
    UNSET_NOT_VAR_AND_FN,
    BAD_VAR_NAME,
    BAD_REDIRECTION,
    BAD_FD,
    // Misuse of shell built-ins -> 2
    ARG_ERROR,
    GRAMMAR_ERROR_ENTRY,
    FORK_ERROR,
    //  Command cannot execute (permission or not an executable) -> 126
    NO_EXEC_PERM,
    NO_READ_PERM,
    INVALID_FILE_PATH,
    // Command not found -> 127
    EXECVP_FAILED
};

/***
 * Clean all of the environment
 * @param ast the ast created by the parser
 */
void clean(struct ast *ast);

/***
 * Cleans the tree, prints the message associated to the error type and exits
 * with the value associated to the error type IF the shell is NOT interactive
 * Do NOT expect this function to exit the program
 * @param type of the error
 */
void exit_gracefully(enum error_type type);

/***
 * Print the appropriate error string on stferr
 * @param the error
 */
void print_error(enum error_type type);

#endif /* !ERROR_HANDLER_H */

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
    FD_DICO_FULL,
    UNEXPECTED_EOF,
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
 * Consume all o he tokens to ensure a proper exit (grammar error)
 */
void comsume_all(void);

/***
 * Cleans the tree, prints the message associatedto the error type and exits
 * with the value associated to the error type
 * @param type of the error
 */
void exit_gracefully(enum error_type type);

/***
 * Print the appropriate error string on stferr
 * @param the error
 */
void print_error(enum error_type type);

#endif /* !ERROR_HANDLER_H */
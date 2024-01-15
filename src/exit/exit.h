#ifndef EXIT_H
#define EXIT_H

#include "tools/ast/ast.h"

enum error_type
{
    ARG_ERROR,
    GRAMMAR_ERROR_ENTRY,
    EXECVP_FAILED,
    RING_BF_FULL,
    MALLOC_NULL,
    ADD_CHILD_ERROR,
    FORK_ERROR,
    LIST_NOT_FOUND,
    INVALID_FILE_PATH,
};

/***
 * Clean all of the environment
 * @param ast the ast created by the parser
 */
void clean(struct ast *ast);

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

#endif /* !EXIT_H */

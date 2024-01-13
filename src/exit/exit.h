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
    FILE_COULDNT_OPEN,
    ADD_CHILD_ERROR,
    FORK_ERROR,
    LIST_NOT_FOUND,
};

/***
 * Clean all of the environment
 * @param ast the ast created by the parser
 */
void clean(struct ast *ast);

/***
 * Print the appropriate error string on stferr
 * @param the error
 */
void print_error(enum error_type type);

#endif /* !EXIT_H */

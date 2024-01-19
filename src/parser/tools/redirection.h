#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "tools/ast/ast.h"
struct redirection
{
    int io_number;
    char *redir;
    char *to;
};

/***
 * Append a redirection allocated to the ast_redir
 * @param ast the ast_redir
 * @param redir the redirection
 */
void append_redir(struct ast_redir *ast, struct redirection *redir);

/***
 * Free all redirections and the list from the redirection ast
 * @param ast the ast
 */
void destroy_redir(struct ast_redir *ast);

#endif /* !REDIRECTION_H */

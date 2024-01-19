#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "tools/ast/ast.h"
struct redirection
{
    int io_number;
    char *redir;
    char *to;
};

void append_redir(struct ast_redir *ast, struct redirection *redir);

#endif /* !REDIRECTION_H */

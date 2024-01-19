#include "redirection.h"

#include <stdlib.h>

void append_redir(struct ast_redir *ast, struct redirection *redir)
{
    ast->redirs =
        realloc(ast->redirs, sizeof(struct redirection *) * ++ast->redir_nb);
    ast->redirs[ast->redir_nb - 1] = redir;
}

void destroy_redir(struct ast_redir *ast)
{
    for (int i = 0; i < ast->redir_nb; i++)
    {
        free(ast->redirs[i]->to);
        free(ast->redirs[i]->redir);
        free(ast->redirs[i]);
    }
    free(ast->redirs);
}

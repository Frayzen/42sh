#include "redirection.h"
#include <stdlib.h>

void append_redir(struct ast_redir *ast, struct redirection* redir)
{
    ast->redirs = realloc(ast->redirs, sizeof(struct redirection *) * ast->redir_nb++);
    ast->redirs[ast->redir_nb] = redir;
}

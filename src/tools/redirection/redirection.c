#include "redirection.h"

#include <assert.h>
#include <stdlib.h>
#include <unistd.h>
#include "env/env.h"

int get_to_fd(struct redirection *redir)
{
    if (redir->type & RT_MASK_DUP && is_number(redir->to))

}

int *setup_redirs(struct ast_redir *ast)
{
    static int saved[3];
    for (int i = 0; i < 3; i++)
        saved[i] = dup(FDS[i]);
    for (int i = 0; i < ast->redir_nb; i++)
    {

        struct redirection* redir = ast->redirs[i];
        int 
    }
    return saved;
}
void close_redirs(int *saved)
{
    for (int i = 0; i < 3; i++)
    {
        dup2(saved[i], FDS[i]);
        close(saved[i]);
    }
}

enum redir_type get_redir_type(char *chevron)
{
    if (chevron[0] == '>')
    {
        switch (chevron[1])
        {
        case '>':
            return RT_APPEND_OUT;
        case '&':
            return RT_DUP_OUT_FD;
        default:
            return RT_REDIR_OUT;
        }
    }
    assert(chevron[0] == '<');
    switch (chevron[1])
    {
    case '>':
        return RT_READ_WRITE;
    case '&':
        return RT_DUP_IN_FD;
    default:
        return RT_REDIR_IN;
    }
}

void append_redir(struct ast_redir *ast, struct redirection *redir)
{
    ast->redirs =
        realloc(ast->redirs, sizeof(struct redirection *) * ++ast->redir_nb);
    ast->redirs[ast->redir_nb - 1] = redir;
}

void destroy_redir(struct ast_redir *ast)
{
    for (int i = 0; i < ast->redir_nb; i++)
    free(ast->redirs);
}

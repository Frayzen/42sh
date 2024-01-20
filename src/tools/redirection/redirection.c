#define _POSIX_C_SOURCE 200809L
#include "redirection.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "env/env.h"
#include "exit/error_handler.h"
#include "tools/str/string.h"

// Return the file descriptor ready to be used
// If we have 1>&2, it duplicates FD[2]
// If we have 2<&1, it duplicates FD[1]
int get_fd(struct redirection *redir)
{
    int fd = -1;
    DBG_PIPE("[REDIR] '%s' has been ", redir->to);
    int type = redir->type;
    if (type & RT_MASK_DUP && is_number(redir->to))
    {
        fd = atoi(redir->to);
        int ret = dup(FDS[fd]);
        DBG_PIPE("duplicated as %d and from %d\n", ret, FDS[fd]);
        return ret;
    }
    else
    {
        DBG_PIPE("considered as a file ");
        int flag = 0;
        if (type & RT_MASK_IN)
            flag |= O_RDONLY;
        else
        {
            flag |= O_WRONLY | O_CREAT;
            if (type == RT_APPEND_OUT)
                flag |= O_APPEND;
            else
                flag |= O_TRUNC;
        }
        fd = open(redir->to, flag, 0644);
        DBG_PIPE("in %d\n", fd);
        return fd;
    }
}

int *setup_redirs(struct ast_redir *ast)
{
    DBG_PIPE("[REDIR] START\n");
    static int saved[3];
    for (int i = 0; i < 3; i++)
    {
        saved[i] = dup(FDS[i]);
        DBG_PIPE("[REDIR] Save FDS[%d] to %d\n", i, saved[i]);
    }
    for (int i = 0; i < ast->redir_nb; i++)
    {
        struct redirection *redir = ast->redirs[i];
        int fd_left = redir->io_number;
        if (fd_left == -1)
            fd_left = redir->type & RT_MASK_IN ? STDIN_FILENO : STDOUT_FILENO;
        int fd_right = get_fd(redir);
        if (fd_right == -1)
        {
            // An error happened
            close_redirs(saved);
            print_error(BAD_FD);
            return NULL;
        }
        if (redir->type & RT_MASK_IN)
        {
            DBG_PIPE("[REDIR] Close and copy %d in FD[%d] IN)\n", fd_left,
                     fd_right);
            dup2(fd_left, FDS[fd_right]);
            close(fd_left);
        }
        else
        {
            DBG_PIPE("[REDIR] Close and copy %d in FD[%d] OUT)\n", fd_right,
                     fd_left);
            dup2(fd_right, FDS[fd_left]);
            close(fd_right);
        }
    }
    return saved;
}

void close_redirs(int *saved)
{
    for (int i = 0; i < 3; i++)
    {
        DBG_PIPE("[REDIR] Restore FDS[%d] to %d\n", i, saved[i]);
        dup2(saved[i], FDS[i]);
        close(saved[i]);
    }
    DBG_PIPE("[REDIR] END = \n\n");
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
    {
        free(ast->redirs[i]->to);
        free(ast->redirs[i]);
    }
    free(ast->redirs);
}

void setup_debug_fds(void)
{
    dup2(STDOUT_FILENO, DBG_OUT);
    fcntl(DBG_OUT, F_SETFD, FD_CLOEXEC);
}

void clean_debug_fds(void)
{
    close(DBG_OUT);
}

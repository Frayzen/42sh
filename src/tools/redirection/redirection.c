#define _POSIX_C_SOURCE 200809L
#include "redirection.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env/env.h"
#include "exit/error_handler.h"
#include "parser/command/expander.h"
#include "tools/str/string.h"

#define NO_FD -1

// Return the file descriptor ready to be used
// If we have 1>&2, it duplicates FD[2]
// If we have 2<&1, it duplicates FD[1]
int get_fd(struct redirection *redir)
{
    char **val = expand(&redir->exp);
    assert(val && val[0] && !val[1]);
    char *to = strdup(val[0]);
    destroy_expanded(val);
    int fd = NO_FD;
    DBG_PIPE("[REDIR] '%s' has been ", to);
    int type = redir->type;
    if (type & RT_MASK_DUP && is_number(to))
    {
        fd = atoi(to);
        int ret = dup(FDS[fd]);
        DBG_PIPE("duplicated as %d and from %d\n", ret, FDS[fd]);
        free(to);
        return ret;
    }
    else
    {
        DBG_PIPE("considered as a file ");
        int flag = 0;
        if (type == RT_READ_WRITE)
            flag = O_CREAT | O_APPEND | O_RDWR;
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
        fd = open(to, flag, 0644);
        DBG_PIPE("in %d\n", fd);
        free(to);
        return fd;
    }
}

void apply_redir(int from, int to, char *dbg_msg)
{
    DBG_PIPE(dbg_msg, from, to);
    dup2(from, FDS[to]);
    close(from);
}

// return true if everything has been fine
bool setup_redir(struct redirection *redir)
{
    int fd_left = redir->io_number;
    if (fd_left == NO_FD)
        fd_left = redir->type & RT_MASK_IN ? STDIN_FILENO : STDOUT_FILENO;
    int fd_right = get_fd(redir);
    if (fd_right == NO_FD)
    {
        // An error happened
        print_error(BAD_FD);
        return false;
    }
    apply_redir(fd_right, fd_left, "[REDIR] Close and copy %d in FD[%d] IN)\n");
    return true;
}

int *setup_redirs(struct ast_redir *ast)
{
    DBG_PIPE("[REDIR] START\n");
    int *saved = malloc(sizeof(int) * 3);
    DBG_PIPE("[REDIR] Save ");
    for (int i = 0; i < 3; i++)
    {
        saved[i] = dup(FDS[i]);
        DBG_PIPE("FDS[%d] to %d ; ", i, saved[i]);
    }
    DBG_PIPE("\n\n");
    for (int i = 3; i < 10; i++)
        FDS[i] = NO_FD;
    for (int i = 0; i < ast->redir_nb; i++)
    {
        struct redirection *redir = ast->redirs[i];
        if (!setup_redir(redir))
        {
            close_redirs(saved);
            return NULL;
        }
    }
    return saved;
}

void close_redirs(int *saved)
{
    DBG_PIPE("[REDIR] Restore ");
    for (int i = 0; i < 3; i++)
    {
        DBG_PIPE("FDS[%d] to %d ; ", i, saved[i]);
        dup2(saved[i], FDS[i]);
        close(saved[i]);
    }
    DBG_PIPE("\n\n");
    for (int i = 3; i < 10; i++)
    {
        if (FDS[i] != NO_FD)
        {
            DBG_PIPE("[REDIR] Closing FDS[%d] = %d\n", i, FDS[i]);
            close(FDS[i]);
            FDS[i] = NO_FD;
        }
    }
    DBG_PIPE("[REDIR] END = \n\n");
    free(saved);
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
        clean_expansion(&ast->redirs[i]->exp);
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

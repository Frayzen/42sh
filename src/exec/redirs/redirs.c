#include "redirs.h"

#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "env/env.h"
#include "execs.h"

int create_fd(char *str, bool is_io, int flags)
{
    int ret = -1;
    if (is_io)
        ret = atoi(str);
    else
        ret = open(str, flags, 0644);
    return ret;
}

bool redirect(struct sh_command *cmd, int from, int to, bool dup_io)
{
    int newto = to;
    if (dup_io)
    {
        if (to < 3)
        {
            DBG_PIPE("Replace %d by its stored value : %d\n", to,
                     cmd->redirs_fds[to]);
            to = cmd->redirs_fds[to];
        }
        newto = dup(to);
        DBG_PIPE("Duplicate %d in %d\n", to, newto);
    }
    if (from < 3)
    {
        if (cmd->redirs_fds[from] >= 3)
        {
            DBG_PIPE("Closing %d before storing | ", cmd->redirs_fds[from]);
            close(cmd->redirs_fds[from]);
        }
        DBG_PIPE("Storing %d in fd[%d]\n", newto, from);
        cmd->redirs_fds[from] = newto;
        return true;
    }
    DBG_PIPE("Redirect (%d) to (%d)\n", from, newto);
    return dup2(from, newto) != -1;
}

// ast is the redir ast and redir is the redir struct
void build_redir(struct ast *ast, struct redir *redir)
{
    int i = 0;
    // PARSE FROM
    struct token *token = ast->children[i++]->token;
    if (token->type == CHEVRON)
        redir->left = token->value[0] == '>' ? "1" : "0";
    else
    {
        redir->left = token->value;
        token = ast->children[i++]->token;
    }
    // PARSE CHEVRON
    char sec_char = token->value[1];
    if (token->value[0] == '>')
    {
        redir->dir = LEFT_TO_RIGHT;
        if (sec_char == '>')
            redir->append = true;
    }
    else if (token->value[0] == '<')
        redir->dir = sec_char == '>' ? BOTH_WAY : RIGHT_TO_LEFT;
    else
        assert(false);
    token = ast->children[i++]->token;
    // PARSE TO
    redir->dup_io = sec_char == '&' && is_number(token->value);
    redir->right = token->value;
}

int get_flag(struct redir *redir, bool left)
{
    int flag = 0;
    switch (redir->dir)
    {
    case BOTH_WAY:
        flag |= O_RDWR;
        break;
    case RIGHT_TO_LEFT:
        flag |= (left ? O_WRONLY : O_RDONLY);
        break;
    case LEFT_TO_RIGHT:
        flag |= (left ? O_RDONLY : O_WRONLY);
        flag |= O_CREAT;
        flag |= (redir->append) ? O_APPEND : O_TRUNC;
        break;
    }
    return flag;
}

bool apply_redirection(struct sh_command *cmd, struct ast *redir_ast)
{
    assert(redir_ast->type == AST_REDIR && redir_ast->nb_children >= 2);
    struct redir redir = { 0 };
    build_redir(redir_ast, &redir);
    int from = create_fd(redir.left, true, get_flag(&redir, true));
    int to = create_fd(redir.right, redir.dup_io, get_flag(&redir, false));
    if (from == -1 || to == -1)
        return false;
    DBG_PIPE("Creating redirection from (%d) to (%d)...\n", from, to);
    if (redir.dir == RIGHT_TO_LEFT)
    {
        int tmp_fd = from;
        from = to;
        to = tmp_fd;
    }
    if (!redirect(cmd, from, to, redir.dup_io))
        return false;
    if (redir.dir == BOTH_WAY && !redirect(cmd, to, from, false))
        return false;
    return true;
}

void close_redirs(struct sh_command *cmd)
{
    DBG_PIPE("[REDIR] Cleaning, closing fds: ");
    for (int i = 0; i < 3; i++)
    {
        if (cmd->redirs_fds[i] >= 3)
        {
            close(cmd->redirs_fds[i]);
            DBG_PIPE("%d ", cmd->redirs_fds[i]);
        }
    }
    DBG_PIPE("\n");
}

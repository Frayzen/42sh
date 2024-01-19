#include "redirs.h"

#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "env/env.h"
#include "execs.h"
#include "tools/ast/ast.h"
#include "tools/fd_manager/fd_dictionnary.h"

/* int create_fd(char *str, bool is_io, int flags) */
/* { */
/*     int ret = -1; */
/*     if (is_io) */
/*         ret = atoi(str); */
/*     else */
/*         ret = open(str, flags, 0644); */
/*     return ret; */
/* } */

/* int dup_fd(int fd) */
/* { */
/*     int new = dup(fd); */
/*     dict_push(fd, new); */
/*     return new; */
/* } */

/* bool redirect(struct sh_command *cmd, int from, int to) */
/* { */
/*     if (from < 3) */
/*     { */
/*         cmd->redirs_fds[from] = to; */
/*         return true; */
/*     } */
/*     return dup2(from, to) != -1; */
/* } */

/* void build_redir(struct ast *ast, struct redir *redir) */
/* { */
/*     int i = 0; */
/*     // PARSE FROM */
/*     struct token *token = ast->children[i++]->token; */
/*     if (token->type == CHEVRON) */
/*         redir->left = token->value[0] == '>' ? "1" : "0"; */
/*     else */
/*     { */
/*         redir->left = token->value; */
/*         token = ast->children[i++]->token; */
/*     } */
/*     // PARSE CHEVRON */
/*     char sec_char = token->value[1]; */
/*     if (token->value[0] == '>') */
/*     { */
/*         redir->dir = LEFT_TO_RIGHT; */
/*         if (sec_char == '>') */
/*             redir->append = true; */
/*     } */
/*     else if (token->value[0] == '<') */
/*         redir->dir = sec_char == '>' ? BOTH_WAY : RIGHT_TO_LEFT; */
/*     else */
/*         assert(false); */
/*     if (sec_char == '&') */
/*         redir->dup = true; */
/*     token = ast->children[i++]->token; */
/*     // PARSE TO */
/*     redir->is_io = token->type == IO_NUMBER; */
/*     redir->right = token->value; */
/* } */

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

bool apply_redirection(struct sh_command *cmd, struct ast_redir *redir_ast)
{
    cmd = cmd + 1;
    redir_ast = redir_ast + 1;
    // TODO
    /* assert(redir_ast->type == AST_REDIR && redir_ast->nb_children >= 2); */
    /* struct redir redir = { 0 }; */
    /* build_redir(redir_ast, &redir); */
    /* int from = create_fd(redir.left, true, get_flag(&redir, true)); */
    /* int to = create_fd(redir.right, redir.is_io, get_flag(&redir, false)); */
    /* if (from == -1 || to == -1) */
    /*     return false; */
    /* DBG_PIPE("Created redirection from (%d) to (%d)\n", from, to); */
    /* if (redir.dup) */
    /*     to = dup(to); */
    /* if (redir.dir == RIGHT_TO_LEFT) */
    /* { */
    /*     int tmp_fd = from; */
    /*     from = to; */
    /*     to = tmp_fd; */
    /* } */
    /* if (!redirect(cmd, from, to)) */
    /*     return false; */
    /* if (redir.dir == BOTH_WAY && !redirect(cmd, to, from)) */
    /*     return false; */
    return true;
}

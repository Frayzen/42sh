#ifndef REDIRS_H
#define REDIRS_H

#include "exec/execs.h"
#include "tools/ast/ast.h"

enum direction
{
    RIGHT_TO_LEFT,
    LEFT_TO_RIGHT,
    BOTH_WAY,
};
struct redir
{
    enum direction dir;
    char *left;
    char *right;
    // if redir is >>
    bool append;
    // is redir is (>&) or (<&) AND the right member is an io_nb
    bool dup_io;
};

/*
 * Apply a redirection on command
 * @param cmd the command
 * @param redir the redirection ast to apply
 */
bool apply_redirection(struct sh_command *cmd, struct ast *redir);

#endif /* !REDIRS_H */

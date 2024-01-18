#ifndef REDIRS_H
#define REDIRS_H

#include "execs.h"
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
    bool append;
    bool dup;
    bool is_io;
};

/*
 * Apply a redirection on command
 * @param cmd the command
 * @param redir the redirection ast to apply
 */
bool apply_redirection(struct sh_command *cmd, struct ast *redir);

#endif /* !REDIRS_H */

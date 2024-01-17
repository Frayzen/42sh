#ifndef REDIRS_H
#define REDIRS_H

#include "execs.h"
#include "tools/ast/ast.h"

void apply_redirection(struct sh_command *cmd, struct ast *redir);

#endif /* !REDIRS_H */

#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_prefix(struct ast_cmd *cmd)
{
    return gr_redir(AST_REDIR(cmd));
}

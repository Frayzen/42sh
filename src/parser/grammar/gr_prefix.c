#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"

enum status gr_prefix(struct ast **ast)
{
    GR_DBG_START(Prefix);
    GR_DBG_RET(gr_redir(ast));
}

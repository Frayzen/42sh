#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_prefix(struct ast **ast)
{
    return gr_redir(ast);
}

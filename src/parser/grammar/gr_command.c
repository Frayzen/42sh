#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_command(struct ast **ast)
{
    return gr_simple_command(ast);
}

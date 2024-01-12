#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_pipeline(struct ast **ast)
{
    return gr_command(ast);
}

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_and_or(struct **ast ast)
{
    return gr_pipeline(ast);
}

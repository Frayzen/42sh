#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_input(struct ast **ast)
{
    gr_simple_command(ast);
    if (!tok_peek()->terminal)
    {
        tok_pop();
        destroy_ast(*ast);
        return ERROR;
    }
    return OK;
}

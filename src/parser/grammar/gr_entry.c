#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

enum status gr_input(struct ast **ast)
{
    enum status state = gr_list(ast);
    if (state == ERROR)
    {
        destroy_ast(*ast);
        goto error;
    }
    if (!tok_peek()->terminal)
    {
        tok_pop();
        goto error;
    }
    tok_pop();
    return OK;
error:
    destroy_ast(*ast);
    return ERROR;
}

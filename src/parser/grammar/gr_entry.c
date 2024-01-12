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
        return ERROR;
    }
    if (!tok_peek()->terminal)
    {
        printf("no terminal tok : %s\n", tok_peek()->value);
        tok_pop();
        destroy_ast(*ast);
        return ERROR;
    }
    tok_pop();
    return OK;
}

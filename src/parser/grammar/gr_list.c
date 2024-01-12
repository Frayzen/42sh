#include <stddef.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

enum status gr_list(struct ast **ast)
{
    if (tok_peek()->terminal)
        return OK;
    struct ast *ast_list = init_ast(AST_LIST, NULL);
    enum status state = gr_and_or(&ast_list);
    if (state == ERROR)
        return ERROR;
    tok_pop();
    while (state == OK)
    {
        add_child(*ast, ast_list);
        if (tok_peek()->terminal)
            break;
        if (tok_peek()->type != SEMI_COLON)
        {
            state = ERROR;
            break;
        }
        tok_pop();
        state = gr_and_or(&ast_list);
    }
    if (tok_peek()->type == SEMI_COLON)
        tok_pop();
    *ast = add_child(*ast, ast_list);
    return OK;
}

#include <stddef.h>
#include <stdio.h>

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
    set_ast_root(ast_list);
    enum status state = gr_and_or(&ast_list);
    if (state == ERROR)
    {
        destroy_ast(ast_list);
        return ERROR;
    }
    while (state == OK)
    {
        if (tok_peek()->type != SEMI_COLON)
        {
            break;
        }
        tok_pop_clean();
        state = gr_and_or(&ast_list);
    }
    if (tok_peek()->type == SEMI_COLON)
        tok_pop_clean();
    *ast = add_child(*ast, ast_list);
    return OK;
}

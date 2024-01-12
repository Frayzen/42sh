#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

#include <stddef.h>

enum status gr_line(struct ast **ast)
{
    enum status state = gr_and_or(ast);
    if (state == ERROR)
        return ERROR;
    while (state == OK)
    {
        struct token *token = tok_peek();
        if (token->type != SEMI_COLON)
            return ERROR;
        tok_pop();
        state = gr_and_or(ast);
    }
    struct token *token = tok_peek();
    if (!token || token->type == SEMI_COLON)
        return OK;
    // maybe need to check if there is something after, if yes -> ERROR ?
    return ERROR;
}

enum status gr_list(struct ast **ast)
{
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

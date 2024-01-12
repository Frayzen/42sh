#include <stddef.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

enum status gr_else(struct ast **ast)
{
    // enum status state = OK;
    struct token *token = tok_peek();
    if (token->terminal)
        return OK;
    if (token->type == ELSE)
    {
        tok_pop();
        return gr_compound_list(ast);
    }
    if (token->type == ELIF)
    {
        
        tok_pop();
        struct ast *elif_ast = init_ast(AST_IF, NULL);
        if (gr_compound_list(&elif_ast) == ERROR)
            return ERROR;
        if (tok_peek()->type != THEN)
            return ERROR;
        tok_pop();
        if (gr_compound_list(&elif_ast) == ERROR)
            return ERROR;
        if (tok_peek()->type == ELSE || tok_peek()->type == ELIF)
        {
            if(gr_else(&elif_ast) == ERROR)
                return ERROR;
        }
        *ast = add_child(*ast, elif_ast);
        return OK;
    }
    return ERROR;
}

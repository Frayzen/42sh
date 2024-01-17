#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

enum status gr_element(struct ast **ast)
{
    struct token *token = tok_peek();
    if (token->terminal)
        return ERROR;
    if (IS_WORDABLE(token))
    {
        token->type = WORD;
        tok_pop();
        struct ast *new_ast = init_ast(AST_TOKEN, token);
        *ast = add_child(*ast, new_ast);
        return OK;
    }
    return gr_redir(ast);
    // if (gr_redir(ast) == ERROR)
    // {
    //     tok_pop();
    //     struct ast *new_ast = init_ast(AST_TOKEN, token);
    //     *ast = add_child(*ast, new_ast);
    //     return OK;
    // }
    // return OK;
}

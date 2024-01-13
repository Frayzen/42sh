#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

enum status gr_else(struct ast **ast)
{
    struct token *token = tok_peek();
    if (token->terminal)
        return OK;
    if (token->type == ELSE)
    {
        tok_pop();
        return gr_compound_list(ast);
    }
    if (token->type != ELIF)
    {
        return ERROR;
    }
    tok_pop();
    struct ast *elif_ast = init_ast(AST_IF, NULL);
    CHECK_GOTO(gr_compound_list(&elif_ast) == ERROR, error);
    CHECK_GOTO(tok_peek()->type != THEN, error)
    tok_pop();
    CHECK_GOTO(gr_compound_list(&elif_ast) == ERROR, error);
    if (tok_peek()->type == ELSE || tok_peek()->type == ELIF)
    {
        CHECK_GOTO(gr_else(&elif_ast) == ERROR, error);
    }
    *ast = add_child(*ast, elif_ast);
    return OK;
error:
    destroy_ast(elif_ast);
    return ERROR;
}

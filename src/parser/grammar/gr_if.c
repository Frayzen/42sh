#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

enum status gr_if(struct ast **ast)
{
    struct token *token = tok_peek();
    if (token->type != IF)
        return ERROR;
    tok_pop();
    struct ast *if_ast = init_ast(AST_IF, NULL);
    CHECK_GOTO(gr_compound_list(&if_ast) == ERROR, error);
    CHECK_GOTO(tok_peek()->type != THEN, error);
    tok_pop();
    CHECK_GOTO(gr_compound_list(&if_ast) == ERROR, error);
    if (tok_peek()->type == ELSE || tok_peek()->type == ELIF)
    {
        CHECK_GOTO(gr_else(&if_ast) == ERROR, error);
    }
    CHECK_GOTO(tok_peek()->type != FI, error);

    tok_pop();
    *ast = add_child(*ast, if_ast);
    return OK;

error:
    destroy_ast(if_ast);
    return ERROR;
}

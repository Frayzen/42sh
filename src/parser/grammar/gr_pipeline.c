#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
/*
pipeline = ['!'] command { '|' {'\n'} command } ;
*/
enum status gr_pipeline(struct ast **ast)
{
    struct ast *parent = *ast;
    struct token *token = tok_peek();
    if (token->type == NEGATION)
    {
        struct ast *neg = init_ast(AST_NEGATE, token);
        add_child(*ast, neg);
        parent = neg;
        tok_pop();
    }
    struct ast *pipe_ast = init_ast(AST_PIPE, NULL);
    CHECK_GOTO(gr_command(&pipe_ast) == ERROR, error);
    while (tok_peek()->type == PIPE)
    {
        tok_pop();
        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();
        CHECK_GOTO(gr_command(&pipe_ast), error);
    }
    parent = add_child(parent, pipe_ast);
    return OK;
error:
    destroy_ast(pipe_ast);
    return ERROR;
}

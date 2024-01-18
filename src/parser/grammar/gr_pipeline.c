#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

enum status gr_pipeline(struct ast **ast)
{
    struct ast *pipe_ast = init_ast(AST_PIPE, NULL);
    CHECK_GOTO(gr_command(&pipe_ast) == ERROR, error);
    while (tok_peek()->type == PIPE)
    {
        tok_pop();
        while (tok_peek()->type == NEWLINE)
            tok_pop();
        CHECK_GOTO(gr_command(&pipe_ast), error);
    }
    *ast = add_child(*ast, pipe_ast);
    return OK;
error:
    destroy_ast(pipe_ast);
    return ERROR;
}

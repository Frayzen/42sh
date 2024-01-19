#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"
// and_or = pipeline { ( '&&' | '||' ) {'\n'} pipeline } ;

enum status aux_and_or(struct ast **ast)
{
    struct token *token = tok_peek();
    if (!IS_OPERATOR(token))
        return ERROR;
    struct ast *op_ast = init_ast(token->type == OR ? AST_OR : AST_AND, NULL);
    tok_pop();
    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();
    CHECK_GOTO(gr_pipeline(&op_ast) == ERROR, error);
    *ast = add_child(*ast, op_ast);
error:
    destroy_ast(op_ast);
    return ERROR;
}

enum status gr_and_or(struct ast **ast)
{
    CHECK_GOTO(gr_pipeline(ast) == ERROR, error);
    enum status state = OK;
    while (state == OK)
        state = aux_and_or(ast);
    return OK;
error:
    return ERROR;
}

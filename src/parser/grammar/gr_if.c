#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"
// rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi' ;
enum status gr_if(struct ast_list *list)
{
    if (tok_peek()->type != IF)
        return ERROR;
    tok_pop_clean();
    struct ast_if *ast = init_ast(AST_IF);

    CHECK_GOTO(gr_compound_list(&ast->cond) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != THEN, error);

    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&ast->then) == ERROR, error);
    gr_else(&ast);
    CHECK_GOTO(tok_peek()->type != FI, error);

    tok_pop_clean();
    *ast = add_child(*ast, ast);
    return OK;

error:
    destroy_ast(ast);
    return ERROR;
}

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
/***
 else_clause =
'else' compound_list
| 'elif' compound_list 'then' compound_list [ else_clause ]
;
***/
enum status gr_else(struct ast_if *ast)
{
    struct token *token = tok_peek();
    if (token->type == ELSE)
    {
        tok_pop_clean();
        struct ast_list *fallback = AST_LIST(ast->fallback);
        return gr_compound_list(&fallback);
    }
    if (token->type != ELIF)
        return ERROR;
    tok_pop_clean();
    struct ast_if *elif = init_ast(AST_IF);
    CHECK_GOTO(gr_compound_list(&elif->cond) == ERROR, error);
    CHECK_GOTO(tok_peek()->type != THEN, error)
    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&elif->then) == ERROR, error);
    gr_else(elif);
    ast->fallback = AST(elif);
    return OK;
error:
    destroy_ast(elif);
    return ERROR;
}

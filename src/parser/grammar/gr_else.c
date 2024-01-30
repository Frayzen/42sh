#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
/***
 else_clause =
'else' compound_list
| 'elif' compound_list 'then' compound_list [ else_clause ]
;
***/
enum status gr_else(struct ast_if *ast)
{
    GR_DBG_START(Else);
    struct token *token = tok_peek();
    if (token->type == ELSE)
    {
        tok_pop_clean();
        ast->fallback = init_ast(AST_LIST);
        if (gr_compound_list(AST_LIST(ast->fallback)) == ERROR)
        {
            destroy_ast(ast->fallback);
            ast->fallback = NULL;
            return ERROR;
        }
        return OK;
    }
    if (token->type != ELIF)
        GR_DBG_RET(ERROR);
    tok_pop_clean();
    struct ast_if *elif = init_ast(AST_IF);
    CHECK_GOTO(gr_compound_list(AST_LIST(&elif->cond)) == ERROR, error);
    CHECK_GOTO(tok_peek()->type != THEN, error)
    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(AST_LIST(&elif->then)) == ERROR, error);
    gr_else(elif);
    ast->fallback = AST(elif);
    return OK;
error:
    destroy_ast(elif);
    return ERROR;
}

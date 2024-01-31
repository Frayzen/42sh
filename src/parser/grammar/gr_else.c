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
    GR_START(Else);
    struct token *token = tok_peek();
    if (token->type == ELSE)
    {
        tok_pop_clean();
        ast->fallback = init_ast(AST_LIST);
        if (gr_compound_list(AST_LIST(ast->fallback)) != OK)
        {
            destroy_ast(ast->fallback);
            ast->fallback = NULL;
            GR_RET(ERROR);
        }
        GR_RET(OK);
    }
    if (token->type == ELIF)
    {
        tok_pop_clean();
        struct ast_if *elif = init_ast(AST_IF);
        if (gr_compound_list(AST_LIST(&elif->cond)) != OK)
            GR_RET_CLEAN(ERROR, elif);
        if (tok_peek()->type != THEN)
            GR_RET_CLEAN(ERROR, elif);
        tok_pop_clean();
        if (gr_compound_list(AST_LIST(&elif->then)) != OK)
            GR_RET_CLEAN(ERROR, elif);
        if (gr_else(elif) == ERROR)
            GR_RET_CLEAN(ERROR, elif);
        ast->fallback = AST(elif);
        GR_RET(OK);
    }
    GR_RET(NO_MATCH);
}

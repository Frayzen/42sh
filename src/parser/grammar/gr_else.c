#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
/***
 else_clause =
'else' compound_list
| 'elif' compound_list 'then' compound_list [ else_clause ]
;
***/
enum status gr_else(struct ast **ast)
{
    struct token *token = tok_peek();
    if (token->type == ELSE)
    {
        tok_pop_clean();
        return gr_compound_list(ast);
    }
    if (token->type != ELIF)
        return ERROR;
    tok_pop_clean();
    struct ast *elif_ast = init_ast(AST_IF, NULL);
    CHECK_GOTO(gr_compound_list(&elif_ast) == ERROR, error);
    CHECK_GOTO(tok_peek()->type != THEN, error)
    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&elif_ast) == ERROR, error);
    gr_else(&elif_ast);
    *ast = add_child(*ast, elif_ast);
    return OK;
error:
    destroy_ast(elif_ast);
    return ERROR;
}

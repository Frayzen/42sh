#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"

static enum status gr_loop(struct ast_list *list, enum token_type type,
                           enum ast_type ast_type)
{
    if (tok_peek()->type != type)
        return NO_MATCH;
    tok_pop_clean();
    struct ast_loop *utl_ast = init_ast(ast_type);
    if (gr_compound_list(AST_LIST(&utl_ast->cond)) != OK)
        goto error;
    if (tok_peek()->type != DO)
        goto error;
    tok_pop_clean();
    if (gr_compound_list(AST_LIST(&utl_ast->exec)) != OK)
        goto error;
    if (tok_peek()->type != DONE)
        goto error;
    tok_pop_clean();
    add_child(list, AST(utl_ast));
    return OK;
error:
    destroy_ast(utl_ast);
    return ERROR;
}

// rule_while = 'while' compound_list 'do' compound_list 'done' ;
enum status gr_while(struct ast_list *ast)
{
    GR_START(While);
    GR_RET(gr_loop(ast, WHILE, AST_WHILE));
}

// rule_until = 'until' compound_list 'do' compound_list 'done' ;
enum status gr_until(struct ast_list *ast)
{
    GR_START(Until);
    GR_RET(gr_loop(ast, UNTIL, AST_UNTIL));
}

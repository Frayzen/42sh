#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"

static enum status gr_loop(struct ast_list *list, enum token_type type)
{
    GR_START(Until);
    if (tok_peek()->type != type)
        GR_RET(NO_MATCH);
    tok_pop_clean();
    struct ast_loop *utl_ast = init_ast(AST_UNTIL);
    if (gr_compound_list(AST_LIST(&utl_ast->cond)) != OK)
        GR_RET_CLEAN(ERROR, utl_ast);
    if (tok_peek()->type != DO)
        GR_RET_CLEAN(ERROR, utl_ast);
    tok_pop_clean();
    if (gr_compound_list(AST_LIST(&utl_ast->exec)) != OK)
        GR_RET_CLEAN(ERROR, utl_ast);
    if (tok_peek()->type != DONE)
        GR_RET_CLEAN(ERROR, utl_ast);
    tok_pop_clean();
    add_child(list, AST(utl_ast));
    GR_RET(OK);
}

// rule_while = 'while' compound_list 'do' compound_list 'done' ;
enum status gr_while(struct ast_list *ast)
{
    return gr_loop(ast, UNTIL);
}

// rule_until = 'until' compound_list 'do' compound_list 'done' ;
enum status gr_until(struct ast_list *ast)
{
    return gr_loop(ast, UNTIL);
}

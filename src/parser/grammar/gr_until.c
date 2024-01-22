#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"
// rule_until = 'until' compound_list 'do' compound_list 'done' ;

enum status gr_until(struct ast_list *ast)
{
    GR_DBG_START(SimpleCommand);
    struct ast_loop *utl_ast = init_ast(AST_UNTIL);

    CHECK_GOTO(tok_peek()->type != UNTIL, error);
    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(AST_LIST(&utl_ast->cond)) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != DO, error);

    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(AST_LIST(&utl_ast->exec)) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != DONE, error);
    tok_pop_clean();

    add_child(ast, AST(utl_ast));
    GR_DBG_RET(OK);
error:
    destroy_ast(utl_ast);
    GR_DBG_RET(ERROR);
}

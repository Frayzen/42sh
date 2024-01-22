#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"

// rule_while = 'while' compound_list 'do' compound_list 'done' ;

enum status gr_while(struct ast_list *list)
{
    GR_DBG_START(SimpleCommand);
    struct ast_loop *wh_ast = init_ast(AST_WHILE);

    CHECK_GOTO(tok_peek()->type != WHILE, error);
    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(AST_LIST(&wh_ast->cond)) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != DO, error);

    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(AST_LIST(&wh_ast->exec)) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != DONE, error);
    tok_pop_clean();

    add_child(list, AST(wh_ast));
    GR_DBG_RET(OK);
error:
    destroy_ast(wh_ast);
    GR_DBG_RET(ERROR);
}

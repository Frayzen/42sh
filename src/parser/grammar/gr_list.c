#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/token/token.h"

/*
list = and_or { ';' and_or } [ ';' ] ;
*/
enum status gr_list(struct ast **new_list)
{
    GR_DBG_START(List);
    struct ast_list *list = init_ast(AST_LIST);
    CHECK_GOTO(gr_and_or(list) == ERROR, error);
    enum status state = OK;
    while (state == OK)
    {
        if (tok_peek()->type != SEMI_COLON)
            break;
        tok_pop_clean();
        state = gr_and_or(list);
    }
    if (tok_peek()->type == SEMI_COLON)
        tok_pop_clean();
    *new_list = AST(list);
    GR_DBG_RET(OK);
error:
    destroy_ast(list);
    GR_DBG_RET(ERROR);
}

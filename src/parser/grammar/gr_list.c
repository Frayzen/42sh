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
    GR_START(List);
    struct ast_list *list = init_ast(AST_LIST);
    enum status status = gr_and_or(list);
    switch (status)
    {
    default:
        GR_RET_CLEAN(status, list);
    case OK:
        break;
    }
    while (status == OK)
    {
        if (tok_peek()->type != SEMI_COLON)
            break;
        tok_pop_clean();
        status = gr_and_or(list);
    }
    if (status == ERROR)
        GR_RET_CLEAN(ERROR, list);
    *new_list = AST(list);
    GR_RET(OK);
}

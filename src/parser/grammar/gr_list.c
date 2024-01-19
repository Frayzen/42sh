#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

/*
list = and_or { ';' and_or } [ ';' ] ;
*/
enum status gr_list(void)
{
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
    return OK;
error:
    destroy_ast(list);
    return ERROR;
}

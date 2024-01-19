#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/gr_tools.h"
#include "tools/token/token.h"

/*
list = and_or { ';' and_or } [ ';' ] ;
*/
enum status gr_list(struct ast **ast)
{
    GR_DBG_START(List);
    struct ast *ast_list = init_ast(AST_LIST, NULL);
    CHECK_GOTO(gr_and_or(&ast_list) == ERROR, error);
    enum status state = OK;
    while (state == OK)
    {
        if (tok_peek()->type != SEMI_COLON)
            break;
        tok_pop_clean();
        state = gr_and_or(&ast_list);
    }
    if (tok_peek()->type == SEMI_COLON)
        tok_pop_clean();
    *ast = add_child(*ast, ast_list);
    GR_DBG_RET(OK);
error:
    destroy_ast(ast_list);
    GR_DBG_RET(ERROR);
}

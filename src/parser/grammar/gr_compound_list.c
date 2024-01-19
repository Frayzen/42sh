#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"
/*
compound_list =
{'\n'} and_or { ( ';' | '\n' ) {'\n'} and_or } [';'] {'\n'} ;
*/
enum status gr_compound_list(struct ast_list **ast)
{
    GR_DBG_START(CompoundList);
    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();
    struct ast_list *ls_ast = init_ast(AST_LIST);
    enum status state = gr_and_or(ls_ast);
    CHECK_GOTO(state == ERROR, error);
    while (state == OK)
    {
        if (tok_peek()->type != NEWLINE && tok_peek()->type != SEMI_COLON)
            break;
        tok_pop_clean();
        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();
        state = gr_and_or(ls_ast);
    }

    if (tok_peek()->type == SEMI_COLON)
        tok_pop_clean();

    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();

    *ast = ls_ast;
    return OK;

error:
    destroy_ast(ls_ast);
    GR_DBG_RET(ERROR);
}

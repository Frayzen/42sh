#include <stddef.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

enum status gr_compound_list(struct ast **ast)
{
    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();
    struct ast *ls_ast = init_ast(AST_LIST, NULL);
    enum status state = gr_and_or(&ls_ast);
    CHECK_GOTO(state == ERROR, error);
    while (state == OK)
    {
        if (tok_peek()->type != NEWLINE && tok_peek()->type != SEMI_COLON)
            break;
        tok_pop_clean();
        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();
        state = gr_and_or(&ls_ast);
    }

    if (tok_peek()->type == SEMI_COLON)
        tok_pop_clean();

    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();

    *ast = add_child(*ast, ls_ast);
    return OK;

error:
    destroy_ast(ls_ast);
    return ERROR;
}

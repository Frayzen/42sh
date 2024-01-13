#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

enum status gr_compound_list(struct ast **ast)
{
    while (tok_peek()->type == NEWLINE)
        tok_pop();
    struct ast *ls_ast = init_ast(AST_LIST, NULL);
    enum status state = gr_and_or(&ls_ast);
    CHECK_GOTO(state == ERROR, error);
    while (state == OK)
    {
        if (tok_peek()->type != NEWLINE && tok_peek()->type != SEMI_COLON)
            break;
        tok_pop();
        while (tok_peek()->type == NEWLINE)
            tok_pop();
        state = gr_and_or(&ls_ast);
    }

    if (tok_peek()->type == SEMI_COLON)
        tok_pop();

    while (tok_peek()->type == NEWLINE)
        tok_pop();

    *ast = add_child(*ast, ls_ast);
    return OK;

error:
    destroy_ast(ls_ast);
    return ERROR;
}

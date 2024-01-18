#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

// rule_until = 'until' compound_list 'do' compound_list 'done' ;
enum status gr_until(struct ast **ast)
{
    struct ast *ut_ast = init_ast(AST_UNTIL, NULL);
    CHECK_GOTO(tok_peek()->type == UNTIL, error);
    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&ut_ast) == ERROR, error);

    CHECK_GOTO(tok_peek()->type == DO, error);
    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&ut_ast) == ERROR, error);

    CHECK_GOTO(tok_peek()->type == DONE, error);
    tok_pop_clean();

    *ast = add_child(*ast, ut_ast);
    return OK;
error:
    destroy_ast(ut_ast);
    return ERROR;
}

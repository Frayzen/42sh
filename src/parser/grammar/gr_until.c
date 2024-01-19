
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

// rule_until = 'until' compound_list 'do' compound_list 'done' ;

enum status gr_until(struct ast **ast)
{
    struct ast *utl_ast = init_ast(AST_UNTIL, NULL);

    CHECK_GOTO(tok_peek()->type != UNTIL, error);
    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&utl_ast) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != DO, error);

    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&utl_ast) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != DONE, error);
    tok_pop_clean();

    *ast = add_child(*ast, utl_ast);
    return OK;
error:
    destroy_ast(utl_ast);
    return ERROR;
}

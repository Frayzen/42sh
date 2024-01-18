#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

// rule_while = 'while' compound_list 'do' compound_list 'done' ;

enum status gr_while(struct ast **ast)
{
    struct ast *wh_ast = init_ast(AST_WHILE, NULL);

    CHECK_GOTO(tok_peek()->type != WHILE, error);
    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&wh_ast) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != DO, error);

    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&wh_ast) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != DONE, error);
    tok_pop_clean();

    *ast = add_child(*ast, wh_ast);
    return OK;
error:
    destroy_ast(wh_ast);
    return ERROR;
}

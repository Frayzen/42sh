#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/gr_tools.h"
#include "tools/token/token.h"
// rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi' ;
enum status gr_if(struct ast **ast)
{
    GR_DBG_START(If);
    if (tok_peek()->type != IF)
        GR_DBG_RET(ERROR);
    tok_pop_clean();
    struct ast *if_ast = init_ast(AST_IF, NULL);

    CHECK_GOTO(gr_compound_list(&if_ast) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != THEN, error);

    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&if_ast) == ERROR, error);
    gr_else(&if_ast);
    CHECK_GOTO(tok_peek()->type != FI, error);

    tok_pop_clean();
    *ast = add_child(*ast, if_ast);
    GR_DBG_RET(OK);

error:
    destroy_ast(if_ast);
    GR_DBG_RET(ERROR);
}

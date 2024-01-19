#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"
// rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi' ;
enum status gr_if(struct ast_list *list)
{
    if (tok_peek()->type != IF)
        return ERROR;
    tok_pop_clean();
    struct ast_if *if_ast = init_ast(AST_IF);

    CHECK_GOTO(gr_compound_list(&if_ast->cond) == ERROR, error);

    CHECK_GOTO(tok_peek()->type != THEN, error);

    tok_pop_clean();
    CHECK_GOTO(gr_compound_list(&if_ast->then) == ERROR, error);
    gr_else(if_ast);
    CHECK_GOTO(tok_peek()->type != FI, error);

    tok_pop_clean();
    add_child(list, AST(if_ast));
    return OK;

error:
    destroy_ast(if_ast);
    return ERROR;
}

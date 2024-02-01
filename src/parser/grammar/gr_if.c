#include "lexer/token_saver.h"
#include "parser/tools/gr_tools.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"
// rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi' ;
enum status gr_if(struct ast_sh *sh)
{
    GR_START(If);
    if (tok_peek()->type != IF)
        GR_RET(NO_MATCH);
    tok_pop_clean();
    struct ast_if *if_ast = init_ast(AST_IF);
    if (gr_compound_list(AST_LIST(&if_ast->cond)) != OK)
        GR_RET_CLEAN(ERROR, if_ast);
    if (tok_peek()->type != THEN)
        GR_RET_CLEAN(ERROR, if_ast);
    tok_pop_clean();
    if (gr_compound_list(AST_LIST(&if_ast->then)) != OK)
        GR_RET_CLEAN(ERROR, if_ast);
    if (gr_else(if_ast) == ERROR)
        GR_RET_CLEAN(ERROR, if_ast);
    if (tok_peek()->type != FI)
        GR_RET_CLEAN(ERROR, if_ast);
    tok_pop_clean();
    sh->sh_cmd = AST(if_ast);
    GR_RET(OK);
}

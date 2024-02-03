#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>

#include "lexer/token_saver.h"
#include "parser/command/expander.h"
#include "parser/command/expansion.h"
#include "parser/grammar/rules.h"
#include "parser/tools/gr_tools.h"
#include "parser/tools/gr_utils.h"
#include "tools/ast/ast.h"
#include "tools/str/string.h"
#include "tools/token/token.h"

// rule_case = 'case' WORD {'\n'} 'in' {'\n'} [case_clause] 'esac' ;

enum status gr_case(struct ast_sh *sh)
{
    GR_START(Case);

    if (tok_peek()->type != CASE)
        GR_RET(NO_MATCH);
    tok_pop_clean();

    if (!IS_WORDABLE(tok_peek()))
        GR_RET(ERROR);
    struct ast_case *case_ast = init_ast(AST_CASE);
    exp_register_str(&case_ast->name, tok_peek()->str);
    tok_pop();

    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();

    if (tok_peek()->type != IN)
        GR_RET_CLEAN(ERROR, case_ast);
    tok_pop_clean();

    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();

    if (gr_case_clause(case_ast) == ERROR)
        GR_RET_CLEAN(ERROR, case_ast);

    if (tok_peek()->type != ESAC)
        GR_RET_CLEAN(ERROR, case_ast);

    tok_pop_clean();
    sh->sh_cmd = AST(case_ast);
    GR_RET(OK);
}

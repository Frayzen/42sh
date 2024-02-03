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

// case_clause = case_item { ';;' {'\n'} case_item } [';;'] {'\n'} ;

enum status gr_case_clause(struct ast_case *ast)
{
    if (tok_peek()->type == ESAC)
        GR_RET(OK);
    ast->nb_cond++;
    ast->list_cond = realloc(ast->list_cond, sizeof(char *) * ast->nb_cond);
    if (gr_case_item(ast) == ERROR)
        GR_RET(ERROR);

    while (tok_peek()->type == DBL_SEMI_COLON)
    {
        tok_pop_clean();
        if (tok_peek()->type == ESAC)
            break;

        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();

        ast->nb_cond++;
        ast->list_cond = realloc(ast->list_cond, sizeof(char *) * ast->nb_cond);
        if (gr_case_item(ast) == ERROR)
            GR_RET(ERROR);
    }

    if (tok_peek()->type == DBL_SEMI_COLON)
        tok_pop_clean();

    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();

    GR_RET(OK);
}

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/gr_tools.h"
#include "tools/token/token.h"

/* rule_for ='for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] )
 * {'\n'} 'do' compound_list 'done';
 */
enum status gr_for(struct ast **ast)
{
    CHECK_GOTO(tok_peek()->type != FOR, error)
    tok_pop_clean();
    CHECK_GOTO(!IS_WORDABLE(tok_peek()), error);
    if (tok_peek()->type == SEMI_COLON)
        tok_pop_clean();
    else
    {
        while (tok_peek()->type == NEWLINE)
            tok_pop_clean();
        if (tok_peek()->type == IN)
        {
            tok_pop_clean();
            while (IS_WORDABLE(tok_peek()))
                tok_pop_clean();
            if (tok_peek()->type != SEMI_COLON)
                CHECK_GOTO(tok_peek()->type == NEWLINE, error);
            tok_pop_clean();
        }
    }
    while (tok_peek()->type == NEWLINE)
        tok_pop_clean();
    CHECK_GOTO(tok_peek()->type != DO, error);
    CHECK_GOTO(gr_compound_list(ast), error);
    CHECK_GOTO(tok_peek()->type != DONE, error);
    GR_DBG_RET(OK);
error:
    GR_DBG_RET(ERROR);
}

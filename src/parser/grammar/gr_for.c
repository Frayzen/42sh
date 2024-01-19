#include "parser/grammar/rules.h"
#include "tools/gr_tools.h"

/* rule_for ='for' WORD ( [';'] | [ {'\n'} 'in' { WORD } ( ';' | '\n' ) ] )
 * {'\n'} 'do' compound_list 'done';
 */
enum status gr_for(ast)
{
    CHECK_GOTO(tok_peek()->type != FOR, ERROR)
    tok_pop_clean();
    GR_DBG_RET(OK);
error:
    GR_DBG_RET(ERROR);
}

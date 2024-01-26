#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"

#define STOP 0 // grammar fails
#define GOOD 1 // grammar is good
#define WAIT 2 // need to look for other grammar

/*
shell_command =
  '{' compound_list '}'
| '(' compound_list ')'
| rule_for
| rule_while
| rule_until
| rule_if
;
*/

int check_compound_token(enum token_type expected_type1,
                         enum token_type expected_type2, struct ast_list *list)
{
    struct token *token = tok_peek();
    if (token->type == expected_type1)
    {
        tok_pop_clean();
        if (gr_list((struct ast **)&list) == OK)
        {
            token = tok_peek();
            if (token->type != expected_type2)
                return STOP;
            tok_pop_clean();
            return GOOD;
        }
        else
            return 0;
    }
    return WAIT;
}

enum status gr_shell_cmd(struct ast_list *list)
{
    GR_DBG_START(ShellCmd);

    int res = check_compound_token(BRK_OPEN, BRK_CLOSED, list);
    if (res == GOOD || res == STOP)
        return (res == GOOD) ? gr_debug_end(OK) : gr_debug_end(ERROR);
    res = check_compound_token(PRTH_OPEN, PRTH_CLOSED, list);
    if (res == GOOD || res == STOP)
        return (res == GOOD) ? gr_debug_end(OK) : gr_debug_end(ERROR);
    if (gr_if(list) == OK)
        GR_DBG_RET(OK);
    if (gr_while(list) == OK)
        GR_DBG_RET(OK);
    if (gr_until(list) == OK)
        GR_DBG_RET(OK);
    if (gr_for(list) == OK)
        GR_DBG_RET(OK);
    GR_DBG_RET(ERROR);
}

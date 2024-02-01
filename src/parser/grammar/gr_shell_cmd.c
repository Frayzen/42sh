#include <stdbool.h>

#include "lexer/token_saver.h"
#include "parser/tools/gr_tools.h"
#include "rules.h"
#include "tools/ast/ast.h"

/*
shell_command =
  '{' compound_list '}'
| '(' compound_list ')'
| rule_for
| rule_while
| rule_until
| rule_case
| rule_if
;
*/

enum status check_compound_token(enum token_type expected_type1,
                                 enum token_type expected_type2,
                                 struct ast_list *list)
{
    struct token *token = tok_peek();
    if (token->type == expected_type1)
    {
        tok_pop_clean();
        if (gr_list((struct ast **)&list) == OK)
        {
            token = tok_peek();
            if (token->type != expected_type2)
                return ERROR;
            tok_pop_clean();
            return OK;
        }
        else
            return ERROR;
    }
    return NO_MATCH;
}

// Function to verify if the return value is a value that needs to be returned
// Return false if a return should not occur
bool checkout(enum status st, enum status *ret)
{
    switch (st)
    {
    case NO_MATCH:
        return false;
    default:
        *ret = st;
        return true;
    }
}

enum status gr_shell_cmd(struct ast_list *list)
{
    GR_START(ShellCmd);
    enum status ret = OK;
    if (checkout(check_compound_token(BRK_OPEN, BRK_CLOSED, list), &ret))
        GR_RET(ret);
    if (checkout(check_compound_token(PRTH_OPEN, PRTH_CLOSED, list), &ret))
        GR_RET(ret);
    if (checkout(gr_if(list), &ret))
        GR_RET(ret);
    if (checkout(gr_while(list), &ret))
        GR_RET(ret);
    if (checkout(gr_until(list), &ret))
        GR_RET(ret);
    if (checkout(gr_for(list), &ret))
        GR_RET(ret);
    if (checkout(gr_case(list), &ret))
        GR_RET(ret);
    GR_RET(NO_MATCH);
}

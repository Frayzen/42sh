#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"

enum status gr_shell_cmd(struct ast_list *list)
{
    GR_DBG_START(ShellCmd);
    struct token *token = tok_peek();
    if (token->type == BRACKET_OPEN)
    {
        tok_pop_clean();
        if (gr_list(list) == OK)
        {
            token = tok_peek();
            if (token->type != BRACKET_CLOSED)
                GR_DBG_RET(ERROR);
            tok_pop_clean();
            return OK;
        }
        else
            GR_DGB_RET(ERROR);
    }
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

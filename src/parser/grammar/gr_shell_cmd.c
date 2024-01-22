#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"

enum status gr_shell_cmd(struct ast_list *list)
{
    struct token *token = tok_peek();
    if (!strcmp(token->value, "{"))
    {
        tok_pop_clean();
        if (gr_list(list) == OK)
        {
            token = tok_peek();
            if (!strcmp(token->value, "}"))
            {
                tok_pop_clean();
                return OK;
            }
        }
    }
    if (gr_if(list) == OK)
        return OK;
    if (gr_while(list) == OK)
        return OK;
    if (gr_until(list) == OK)
        return OK;
    if (gr_for(list) == OK)
        return OK;

    GR_DBG_RET(ERROR);
}

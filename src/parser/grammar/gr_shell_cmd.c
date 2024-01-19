#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_shell_cmd(struct ast_list *list)
{
    if (gr_if(list) == OK)
        return OK;
    if (gr_while(list) == OK)
        return OK;
    if (gr_until(list) == OK)
        return OK;

    GR_DBG_RET(ERROR);
}

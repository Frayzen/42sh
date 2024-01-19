#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
/*
command =
simple_command
| shell_command { redirection }
;
*/
enum status gr_command(struct ast **ast)
{
    GR_DBG_START(Command);
    if (gr_simple_command(ast) == OK)
        GR_DBG_RET(OK);
    if (gr_shell_cmd(ast) == ERROR)
        GR_DBG_RET(ERROR);
    while (gr_redir(ast) == OK)
        ;
    GR_DBG_RET(OK);
}

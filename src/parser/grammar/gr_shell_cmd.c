#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/gr_tools.h"

enum status gr_shell_cmd(struct ast **ast)
{
    GR_DBG_START(ShellCmd);
    if (gr_if(ast) == OK)
        GR_DBG_RET(OK);
    if (gr_while(ast) == OK)
        GR_DBG_RET(OK);
    if (gr_until(ast) == OK)
        GR_DBG_RET(OK);

    GR_DBG_RET(ERROR);
}

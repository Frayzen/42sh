#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
/*
command =
simple_command
| shell_command { redirection }
;
*/
enum status gr_command(struct ast **ast)
{
    if (gr_simple_command(ast) == OK)
        return OK;
    if (gr_shell_cmd(ast) == ERROR)
        return ERROR;
    enum status state = OK;
    while (state == OK)
    {
        if (tok_peek()->terminal)
            break;
        state = gr_redir(ast);
    }
    return OK;
}

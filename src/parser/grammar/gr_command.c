#include "rules.h"
#include "tools/ast/ast.h"
/*
command =
simple_command
| shell_command { redirection }
;
*/
enum status gr_command(struct ast_pipe *pipe)
{
    struct ast_list *list = AST_LIST(pipe);
    if (gr_simple_command(list) == OK)
        return OK;
    if (gr_shell_cmd(list) == ERROR)
        return ERROR;
    struct ast_sh* sh = AST_SH(list->children[list->nb_children]);
    while (gr_redir(AST_REDIR(sh)) == OK)
        ;
    return OK;
}

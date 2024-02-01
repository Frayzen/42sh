#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
/*
command =
simple_command
| shell_command { redirection }
;
*/
enum status gr_command(struct ast_pipe *pipe)
{
    GR_START(Command);
    struct ast_list *list = AST_LIST(pipe);
    switch (gr_simple_command(list))
    {
    case OK:
        GR_RET(OK);
    case ERROR:
        GR_RET(ERROR);
    default:
        break;
    }
    enum status st = gr_shell_cmd(list);
    switch (st)
    {
    case OK: {
        struct ast_sh *sh = AST_SH(list->children[list->nb_children - 1]);
        while (gr_redir(AST_REDIR(sh)) == OK)
            ;
        GR_RET(OK);
    }
    default:
        break;
    }
    st = gr_function(list);
    switch (st)
    {
    case OK: {
        struct ast_sh *sh = AST_SH(list->children[list->nb_children - 1]);
        while (gr_redir(AST_REDIR(sh)) == OK)
            ;
        GR_RET(OK);
    }
    default:
        GR_RET(st);
    }
}

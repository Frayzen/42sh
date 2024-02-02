#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
/*
command =
simple_command
| shell_command { redirection }
;
*/

enum status consume_redirs(struct ast_redir *redir)
{
    enum status st;

    do
        st = gr_redir(AST_REDIR(redir));
    while (st == OK);
    return st == ERROR ? ERROR : OK;
}

#define LAST_CHILD(List) ((List)->children[(List)->nb_children - 1])
enum status gr_command(struct ast_pipe *pipe)
{
    GR_START(Command);
    enum status st;
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
    st = gr_shell_cmd(AST(list));
    if (st == ERROR)
        GR_RET(ERROR);
    if (st == OK)
        return consume_redirs(AST_REDIR(LAST_CHILD(list)));
    st = gr_function(list);
    if (st == ERROR)
        GR_RET(ERROR);
    if (st == OK)
        return consume_redirs(AST_REDIR(LAST_CHILD(list)));
    GR_RET(NO_MATCH);
}

#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
/*
command =
simple_command
| shell_command { redirection }
| func_def { redirection }
;
*/

enum status consume_redirs(struct ast_redir *sh)
{
    enum status st;
    do
        st = gr_redir(AST_REDIR(sh));
    while (st == OK);
    return st == ERROR ? ERROR : OK;
}

#define LAST_CHILD(List) ((List)->children[(List)->nb_children - 1])
enum status gr_command(struct ast_pipe *pipe)
{
    GR_START(Command);
    struct ast_list *list = AST_LIST(pipe);
    enum status st = gr_simple_command(list);
    if (st != NO_MATCH)
        GR_RET(st);

    // shell function
    st = gr_shell_cmd(AST(list));
    if (st == ERROR)
        GR_RET(ERROR);
    if (st == OK)
        GR_RET(consume_redirs(AST_REDIR(LAST_CHILD(list))));

    // func_def
    st = gr_function(list);
    if (st == ERROR)
        GR_RET(ERROR);
    if (st == OK)
        GR_RET(consume_redirs(AST_REDIR(LAST_CHILD(list))));
    GR_RET(NO_MATCH);
}

#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
/*
funcdec = WORD '(' ')' '{\n}' shell_command
*/
enum status gr_function(struct ast_pipe *pipe)
{
    GR_DBG_START(Command);
    struct ast_func *struct token *token = tok_peek();
    if (!IS_WORDABLE(token))
        return ERROR;
    tok_pop_clean();
    i

        struct ast_list *list = AST_LIST(pipe);
    if (gr_simple_command(list) == OK)
        return OK;
    if (gr_shell_cmd(list) == OK)
    {
        struct ast_sh *sh = AST_SH(list->children[list->nb_children - 1]);
        while (gr_redir(AST_REDIR(sh)) == OK)
            ;
        return OK;
    }
    if (gr_function(list) == OK)
    {
        while (gr_redir(AST_REDIR(sh)) == OK)
            ;
        return OK;
    }
    GR_DBG_RET(ERROR);
}

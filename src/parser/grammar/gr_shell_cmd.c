#include <stdbool.h>

#include "lexer/token_saver.h"
#include "parser/tools/gr_tools.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"

/*
shell_command =
  '{' compound_list '}'
| '(' compound_list ')'
| rule_for
| rule_while
| rule_until
| rule_case
| rule_if
;
*/

enum status check_compound_token(enum token_type expected_type1,
                                 enum token_type expected_type2,
                                 enum ast_type type, struct ast_sh *sh)
{
    struct token *token = tok_peek();
    if (token->type != expected_type1)
        return NO_MATCH;
    struct ast_list *sub_shell = init_ast(type);
    tok_pop_clean();
    if (gr_compound_list(AST_LIST(sub_shell)) != OK)
    {
        destroy_ast(sub_shell);
        return ERROR;
    }
    token = tok_peek();
    if (token->type != expected_type2)
    {
        destroy_ast(sub_shell);
        return ERROR;
    }
    tok_pop_clean();
    sh->sh_cmd = AST(sub_shell);
    return OK;
}

// Function to verify if the return value is a value that needs to be returned
// Return false if a return should not occur
bool checkout(enum status st, enum status *ret)
{
    switch (st)
    {
    case NO_MATCH:
        return false;
    default:
        *ret = st;
        return true;
    }
}

enum status get_shell_command(struct ast_sh *sh)
{
    enum status ret = OK;
    if (checkout(check_compound_token(BRK_OPEN, BRK_CLOSED, AST_LIST, sh),
                 &ret))
        return ret;
    if (checkout(check_compound_token(PRTH_OPEN, PRTH_CLOSED, AST_SUBSHELL, sh),
                 &ret))
        return ret;
    if (checkout(gr_if(sh), &ret))
        return ret;
    if (checkout(gr_while(sh), &ret))
        return ret;
    if (checkout(gr_until(sh), &ret))
        return ret;
    if (checkout(gr_for(sh), &ret))
        return ret;
    if (checkout(gr_case(sh), &ret))
        return ret;
    return NO_MATCH;
}

enum status gr_shell_cmd(struct ast_list *list)
{
    GR_START(ShellCmd);
    struct ast_sh *sh = init_ast(AST_SH);
    switch (get_shell_command(sh))
    {
    case NO_MATCH:
        GR_RET_CLEAN(NO_MATCH, sh);
    case ERROR:
        GR_RET_CLEAN(ERROR, sh);
    case OK:
        break;
    }
    add_child(list, AST(sh));
    GR_RET(OK);
}

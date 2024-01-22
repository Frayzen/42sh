#include "env/vars/vars.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"
#include "tools/gr_tools.h"
#include "tools/gr_utils.h"
#include "lexer/token_saver.h"
#include "rules.h"
/*
* prefix =
ASSIGNMENT_WORD
| redirection
;
*/

enum status gr_prefix(struct ast_cmd *cmd)
{
    struct token *token = tok_peek();
    if (token->type == ASSIGNMENT_WORD || token->type == WORD)
    {
        append_ass(&cmd->ass_list, token->str);
        tok_pop();
        GR_DBG_RET(OK);
    }
    return gr_redir(AST_REDIR(cmd));
}

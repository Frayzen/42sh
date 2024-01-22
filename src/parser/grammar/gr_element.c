#include "lexer/token_saver.h"
#include "parser/command/expander.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/token/token.h"

/*
element =
WORD
| redirection
;
*/
enum status gr_element(struct ast_cmd *cmd)
{
    GR_DBG_START(GrElement);
    struct token *token = tok_peek();
    if (IS_WORDABLE(token))
    {
        cmd_register_token(cmd, token);
        tok_pop();
        GR_DBG_RET(OK);
    }
    return gr_redir(AST_REDIR(cmd));
}

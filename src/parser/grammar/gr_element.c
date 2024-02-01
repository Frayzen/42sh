#include "lexer/token_saver.h"
#include "parser/command/expander.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "parser/tools/gr_tools.h"
#include "tools/token/token.h"

/*
element =
WORD
| redirection
;
*/
enum status gr_element(struct ast_cmd *cmd)
{
    GR_START(GrElement);
    struct token *token = tok_peek();
    if (IS_WORDABLE(token))
    {
        exp_register_str(&cmd->args_expansion, token->str);
        tok_pop();
        GR_RET(OK);
    }
    GR_RET(gr_redir(AST_REDIR(cmd)));
}

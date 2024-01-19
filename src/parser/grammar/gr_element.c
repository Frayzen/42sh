#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_utils.h"
#include "tools/token/token.h"

/*
element =
WORD
| redirection
;
*/
enum status gr_element(struct ast_cmd *cmd)
{
    struct token *token = tok_peek();
    if (IS_WORDABLE(token))
    {
        append_arg(cmd, token->value);
        tok_pop();
        return OK;
    }
    return gr_redir(AST_REDIR(cmd));
}

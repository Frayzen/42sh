#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"
#include "lexer/token_saver.h"
#include "parser/command/expander.h"
/*
*prefix =
ASSIGNMENT_WORD
| redirection
;
*/
enum status gr_prefix(struct ast_cmd *cmd)
{
    struct token *token = tok_peek();
    if (token->type == ASSMT)
    {
        exp_register_str(&cmd->args_expansion, token->str);
        tok_pop();

    }
    return gr_redir(AST_REDIR(cmd));
}

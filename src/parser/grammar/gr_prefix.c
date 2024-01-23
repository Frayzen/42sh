#include "rules.h"
#include "tools/assignment/assignment.h"
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
        // exp_register_str(&cmd->args_expansion, token->str);
        struct assignment *ass = init_assignment(token->str);
        ass_list_append(&cmd->assignment_list, ass);
        
        tok_pop();
        return OK;

    }
    return gr_redir(AST_REDIR(cmd));
}

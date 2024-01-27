#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/assignment/assignment.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"
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
        struct assignment *ass = init_assignment(token->str);
        append_ass_list(&cmd->assignment_list, ass);
        tok_pop_clean();
        return OK;
    }
    return gr_redir(AST_REDIR(cmd));
}

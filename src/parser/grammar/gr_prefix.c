#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/assignment/assignment.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/token/token.h"
/*
*prefix =
ASSIGNMENT_WORD
| redirection
;
*/
enum status gr_prefix(struct ast_cmd *cmd)
{
    GR_START(Prefix);
    struct token *token = tok_peek();
    if (token->type == ASSMT)
    {
        struct assignment *ass = init_assignment(token->str);

        if (!ass)
            GR_RET(ERROR);
        append_ass_list(&cmd->assignment_list, ass);
        tok_pop_clean();
        GR_RET(OK);
    }
    GR_RET(gr_redir(AST_REDIR(cmd)));
}

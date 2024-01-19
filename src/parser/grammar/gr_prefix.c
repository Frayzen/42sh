#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/gr_tools.h"
/***
prefix =
ASSIGNMENT_WORD
| redirection
;
*/
enum status gr_prefix(struct ast **ast)
{
    GR_DBG_START(Prefix);
    if (gr_redir(ast) == OK)
        GR_DBG_RET(OK);
    GR_DBG_RET(ERROR);

    struct token *token = tok_peek();
    if (token->type != WORD)
        GR_DBG_RET(ERROR);
    tok_pop();
    token->type = ASSIGNMENT_WORD;
    *ast = add_child(*ast, init_ast(AST_TOKEN, token));
    GR_DBG_RET(OK);
}

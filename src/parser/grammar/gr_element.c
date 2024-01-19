#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/gr_tools.h"
#include "tools/token/token.h"

/*
element =
WORD
| redirection
;
*/
enum status gr_element(struct ast **ast)
{
    GR_DBG_START(GrElement);
    struct token *token = tok_peek();
    if (IS_WORDABLE(token))
    {
        *ast = add_child(*ast, init_ast(AST_TOKEN, token));
        tok_pop();
        GR_DBG_RET(OK);
    }
    GR_DBG_RET(gr_redir(ast));
}

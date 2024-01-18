#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

/*
element =
WORD
| redirection
;
*/
enum status gr_element(struct ast **ast)
{
    struct token *token = tok_peek();
    if (IS_WORDABLE(token))
    {
        *ast = add_child(*ast, init_ast(AST_TOKEN, token));
        tok_pop();
        return OK;
    }
    return gr_redir(ast);
}

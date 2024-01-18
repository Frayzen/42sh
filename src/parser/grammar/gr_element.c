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
    if (gr_redir(ast) == OK)
        return OK;
    if (!IS_WORDABLE(token))
        return ERROR;
    struct ast *ast_tok = init_ast(AST_TOKEN, token);
    *ast = add_child(*ast, ast_tok);
    tok_pop();
    return OK;
}

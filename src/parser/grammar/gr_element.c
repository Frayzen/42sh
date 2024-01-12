#include "tools/ast/ast_utils.h"
#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_element(struct ast **ast)
{
    struct token *token = tok_peek();
    if (is_terminating(token))
        return ERROR;
    token->type = WORD;
    tok_pop();
    struct ast *new_ast = init_ast(token);
    *ast = add_child(*ast, new_ast);
    return OK;
}

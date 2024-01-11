#include "ast/ast.h"
#include "grules.h"

struct ast *grelement(void)
{
    struct token *token = peek_token();
    token->type = WORD;
    pop_token();
    struct ast *ast = init_ast(token);
    return ast;
}

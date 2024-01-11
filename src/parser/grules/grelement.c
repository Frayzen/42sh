#include "grules.h"
#include "lexer/token_saver.h"
#include "tools/ast/ast.h"

struct ast *grelement(void)
{
    struct token *token = tok_peek();
    token->type = WORD;
    tok_pop();
    struct ast *ast = init_ast(token);
    return ast;
}

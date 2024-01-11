#include "tools/ast/ast.h"
#include "lexer/token_saver.h"
#include "grules.h"

struct ast *grelement(void)
{
    struct token *token = tok_peek();
    token->type = WORD;
    tok_pop();
    struct ast *ast = init_ast(token);
    return ast;
}

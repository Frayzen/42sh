#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

struct ast *gr_element(void)
{
    struct token *token = tok_peek();
    token->type = WORD;
    tok_pop();
    struct ast *ast = init_ast(token);
    return ast;
}

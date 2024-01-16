#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

enum status gr_pipeline(struct ast **ast)
{
    struct ast *parent = *ast;
    struct token *token = tok_peek();
    if (token->type == NEGATION)
    {
        struct ast *neg = init_ast(AST_NEGATE, token);
        add_child(*ast, neg);
        parent = neg;
        tok_pop();
    }
    return gr_command(&parent);
}

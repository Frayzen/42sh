#include "ast.h"

#include <stdlib.h>

struct ast *init_ast(enum ast_type type, struct token *token)
{
    struct ast *ast = calloc(1, sizeof(struct ast));
    ast->type = type;
    ast->token = token;
    return ast;
}

void destroy_ast(struct ast *ast)
{
    if (!ast)
        return;
    for (int i = 0; i < ast->nb_children; i++)
        destroy_ast(ast->children[i]);
    free(ast);
}

#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

struct ast **set_ast_root(struct ast **ast)
{
    static struct ast **ast_root = NULL;
    if (ast)
        ast_root = ast;
    return ast_root;
}

void *init_ast(enum ast_type type)
{
    static const size_t ast_size[] = {
        [AST_IF] = sizeof(struct ast_if),
        [AST_LIST] = sizeof(struct ast_list),
        [AST_CMD] = sizeof(struct ast_cmd),
        [AST_PIPE] = sizeof(struct ast_pipe),
        [AST_WHILE] = sizeof(struct ast_loop),
        [AST_UNTIL] = sizeof(struct ast_loop),
        //TODO later
        [AST_ASS] = sizeof(struct ast),
        [AST_AND] = sizeof(struct ast),
        [AST_OR] = sizeof(struct ast),
    };
    struct ast *ast = calloc(1, ast_size[type]);
    ast->type = type;
    return ast;
}

void destroy_ast(void *ast)
{
    if (!ast)
        return;
    switch (AST(ast)->type)
    {
    case AST_CMD:
        free(AST_CMD(ast)->argv);
        break;
    case AST_IF:
            destroy_ast(AST_IF(ast)->cond);
            destroy_ast(AST_IF(ast)->then);
            destroy_ast(AST_IF(ast)->fallback);
        break;
    case AST_LIST:
        for (int i = 0; i < AST_LIST(ast)->nb_children; i++)
            destroy_ast(AST_LIST(ast)->children[i]);
        free(AST_LIST(ast)->children);
        break;
    default:
        break;
    }
    free(ast);
}

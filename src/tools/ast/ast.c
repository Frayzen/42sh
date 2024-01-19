#include "ast.h"

#include <stdio.h>
#include <stdlib.h>
#include "tools/redirection/redirection.h"

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
        [AST_FOR] = sizeof(struct ast_for),
        // TODO later
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
    // might be usefull to switch to get_children but need refacto of the fn
    switch (AST(ast)->type)
    {
    case AST_CMD:
        for (int i = 0; i < AST_CMD(ast)->argc; i++)
            free(AST_CMD(ast)->argv[i]);
        free(AST_CMD(ast)->argv);
        /* FALLTHROUGH */
    case AST_SH:
        destroy_redir(AST_REDIR(ast));
        break;
    case AST_IF:
        destroy_ast(AST_IF(ast)->cond);
        destroy_ast(AST_IF(ast)->then);
        destroy_ast(AST_IF(ast)->fallback);
        break;
    case AST_PIPE:
    case AST_LIST:
        for (int i = 0; i < AST_LIST(ast)->nb_children; i++)
            destroy_ast(AST_LIST(ast)->children[i]);
        free(AST_LIST(ast)->children);
        break;
    case AST_WHILE:
    case AST_UNTIL:
        destroy_ast(AST_LOOP(ast)->cond);
        destroy_ast(AST_LOOP(ast)->exec);
    case AST_FOR:
        destroy_ast(AST_FOR(ast)->cmds);
        if (AST_FOR(ast)->item_list)
            free(AST_FOR(ast)->item_list);
    default:
        break;
    }
    free(ast);
}

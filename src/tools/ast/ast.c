#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

#include "str/string.h"
#include "tools/redirection/redirection.h"

struct ast **swap_ast_root(struct ast **new_ast)
{
    static struct ast **ast_root = NULL;
    struct ast **old_root = ast_root;
    if (new_ast)
    {
        old_root = ast_root;
        ast_root = new_ast;
    }
    return old_root;
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
        [AST_AND_OR] = sizeof(struct ast_and_or),
        // TODO later
        [AST_FUNCT] = sizeof(struct ast_funct),
        [AST_ASS] = sizeof(struct ast),
    };
    struct ast *ast = calloc(1, ast_size[type]);
    ast->type = type;
    return ast;
}

void destroy_list(struct ast_list *list)
{
    if (!list)
        return;
    for (int i = 0; i < list->nb_children; i++)
        destroy_ast(list->children[i]);
    free(list->children);
}

void destroy_ast(void *ast)
{
    if (!ast)
        return;
    // might be usefull to switch to get_children but need refacto of the fn
    switch (AST(ast)->type)
    {
    case AST_CMD:
        clean_expansion(&AST_CMD(ast)->args_expansion);
        /* FALLTHROUGH */
    case AST_SH:
        destroy_redir(AST_REDIR(ast));
        break;
    case AST_WHILE:
    case AST_UNTIL:
        destroy_list(&AST_LOOP(ast)->cond);
        destroy_list(&AST_LOOP(ast)->exec);
        break;
    case AST_IF:
        destroy_list(&AST_IF(ast)->cond);
        destroy_list(&AST_IF(ast)->then);
        destroy_ast(AST_IF(ast)->fallback);
        break;
    case AST_AND_OR:
        free(AST_AND_OR(ast)->types);
        goto destroy_list;
        break;
    case AST_FOR:
        free(AST_FOR(ast)->name);
        if (AST_FOR(ast)->item_list)
        {
            for (int i = 0; i < AST_FOR(ast)->nb_items; i++)
                destroy_lex_str(AST_FOR(ast)->item_list[i]);
            free(AST_FOR(ast)->item_list);
        }
        /* FALLTHROUGH */
    case AST_PIPE:
    case AST_LIST:
    destroy_list:
        destroy_list(AST_LIST(ast));
        break;
    default:
        break;
    }
    free(ast);
}

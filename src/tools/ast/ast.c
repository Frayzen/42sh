#include "ast.h"

#include <stdio.h>
#include <stdlib.h>

#include "parser/command/expander.h"
#include "parser/command/expansion.h"
#include "tools/assignment/assignment.h"
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
        [AST_SUBSHELL] = sizeof(struct ast_subshell),
        [AST_SH] = sizeof(struct ast_sh),
        [AST_CASE] = sizeof(struct ast_case),
        [AST_ASS] = sizeof(struct ast),
        [AST_FUNCT] = sizeof(struct ast_funct),
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

static void destroy_case(struct ast_case *ast)
{
    for (int i = 0; i < ast->nb_cond; i++)
    {
        for (int j = 0; ast->list_cond[i][j]; j++)
        {
            clean_expansion(ast->list_cond[i][j]);
            free(ast->list_cond[i][j]);
        }
        free(ast->list_cond[i]);
        destroy_list(ast->cmds[i]);
        free(ast->cmds[i]);
    }
    free(ast->list_cond);
    free(ast->cmds);
    clean_expansion(&ast->name);
}

void destroy_ast2(void *ast)
{
    if (!ast)
        return;
    // might be usefull to switch to get_children but need refacto of the fn
    switch (AST(ast)->type)
    {
    case AST_AND_OR:
        free(AST_AND_OR(ast)->types);
        goto destroy_list;
        break;
    case AST_CASE:
        destroy_case(AST_CASE(ast));
        break;
    case AST_FUNCT:
        free(AST_FUNCT(ast)->name);
        // don't need to free the body because it is passed to the dictionnary
        // it will be freed when the dictionnary is freed
        break;
    case AST_FOR:
        free(AST_FOR(ast)->name);
        clean_expansion(&AST_FOR(ast)->exp);
        /* FALLTHROUGH */
    case AST_PIPE:
    case AST_SUBSHELL:
    case AST_LIST:
    destroy_list:
        destroy_list(AST_LIST(ast));
        break;
    default:
        break;
    }
    free(ast);
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
        clean_assignments(&AST_CMD(ast)->assignment_list);
        destroy_redir(AST_REDIR(ast));
        break;
    case AST_SH:
        destroy_redir(AST_REDIR(ast));
        destroy_ast(AST_SH(ast)->sh_cmd);
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
    default:
        destroy_ast2(ast);
        return;
    }
    free(ast);
}

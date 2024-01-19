#include "gr_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exit/error_handler.h"
#include "tools/ast/ast.h"

void append_arg(struct ast_cmd *cmd, char *arg)
{
    cmd->argv = realloc(cmd->argv, (++cmd->argc + 1) * (sizeof(char *)));
    cmd->argv[cmd->argc - 1] = arg;
    // To make sure argv is null terminated
    cmd->argv[cmd->argc] = NULL;
}

void add_child(struct ast_list *list, struct ast *child)
{
    if (!list)
        return;
    list->children =
        realloc(list->children, sizeof(struct ast *) * (++list->nb_children));
    if (!list->children)
        exit_gracefully(ADD_CHILD_ERROR);
    list->children[list->nb_children - 1] = child;
}

#define CHILDREN_SIZE 1024
struct ast **get_children(struct ast *ast)
{
    struct ast **ret = calloc(CHILDREN_SIZE, sizeof(struct ast *));
    int i = 0;
    switch (ast->type)
    {
    case AST_PIPE:
    case AST_LIST:
        for (int i = 0; i < AST_LIST(ast)->nb_children; i++)
            ret[i] = AST_LIST(ast)->children[i];
        break;
    case AST_IF:
        ret[i++] = AST(AST_IF(ast)->cond);
        ret[i++] = AST(AST_IF(ast)->then);
        ret[i++] = AST(AST_IF(ast)->fallback);
        break;
    case AST_WHILE:
    case AST_UNTIL:
        ret[i++] = AST(AST_LOOP(ast)->cond);
        ret[i++] = AST(AST_LOOP(ast)->exec);
        break;
    default:
        free(ret);
        return NULL;
    }
    return ret;
}

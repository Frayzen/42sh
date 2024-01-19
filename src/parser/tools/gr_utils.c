#include "gr_utils.h"

#include <stdlib.h>
#include <string.h>

#include "exit/error_handler.h"
#include "tools/ast/ast.h"

void append_arg(struct ast_cmd *cmd, char *arg)
{
    cmd->argv = realloc(cmd->argv, ++cmd->argc * (sizeof(char *)));
    cmd->argv[cmd->argc - 1] = arg;
}

void add_child(struct ast_list *list, struct ast *child)
{
    if (!list)
        return;
    list->children =
        realloc(list->children, sizeof(struct ast) * (list->nb_children + 1));
    if (!list->children)
        exit_gracefully(ADD_CHILD_ERROR);
    list->children[list->nb_children] = child;
    list->nb_children++;
}

struct ast **get_children(struct ast *ast)
{
    static struct ast *ret[1024];
    memset(ret, 0, 1024 * sizeof(struct ast *));
    int i = 0;
    switch (ast->type)
    {
    case AST_LIST:
        memcpy(ret, AST_LIST(ast)->children, AST_LIST(ast)->nb_children);
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
        break;
    }
    return ret;
}

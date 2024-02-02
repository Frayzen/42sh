#include "gr_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exit/error_handler.h"
#include "tools/ast/ast.h"

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
    if (!ast)
        return NULL;
    struct ast **ret = calloc(CHILDREN_SIZE, sizeof(struct ast *));
    int i = 0;
    switch (ast->type)
    {
    case AST_PIPE:
    case AST_FOR:
    case AST_LIST:
    case AST_AND_OR:
    case AST_SUBSHELL:
        for (int i = 0; i < AST_LIST(ast)->nb_children; i++)
            ret[i] = AST_LIST(ast)->children[i];
        break;
    case AST_IF:
        ret[i++] = AST(&AST_IF(ast)->cond);
        ret[i++] = AST(&AST_IF(ast)->then);
        // Normal to not take the pointer, fallback already is
        ret[i++] = AST(AST_IF(ast)->fallback);
        break;
    case AST_WHILE:
    case AST_UNTIL:
        ret[i++] = AST(&AST_LOOP(ast)->cond);
        ret[i++] = AST(&AST_LOOP(ast)->exec);
        break;
    case AST_FUNCT:
        ret[i++] = AST(&AST_FUNCT(ast)->body);
        break;
    default:
        free(ret);
        return NULL;
    }
    return ret;
}

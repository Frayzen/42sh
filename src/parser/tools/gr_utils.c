#include "gr_utils.h"

#include <stdlib.h>
#include "exit/error_handler.h"

void append_arg(struct ast_cmd *cmd, char *arg)
{
    cmd->argv = realloc(cmd->argv, ++cmd->argc * (sizeof(char *)));
    cmd->argv[cmd->argc - 1] = arg;
}

void add_child(struct ast_list *list, struct ast *child)
{
    if (!list)
        return;
    list->children = realloc(list->children,
                               sizeof(struct ast) * (list->nb_children + 1));
    if (!list->children)
        exit_gracefully(ADD_CHILD_ERROR);
    list->children[list->nb_children] = child;
    list->nb_children++;
}


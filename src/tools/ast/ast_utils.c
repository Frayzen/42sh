#include "ast_utils.h"

#include <stdlib.h>

struct ast *add_child(struct ast *parent, struct ast *child)
{
    if (!parent)
        return child;
    parent->children = realloc(parent->children, parent->nb_children + 1);
    parent->children[parent->nb_children] = child;
    parent->nb_children++;
    return parent;
}

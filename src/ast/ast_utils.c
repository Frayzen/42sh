#include "ast_utils.h"

#include <stddef.h>
#include <stdlib.h>

struct ast *add_child(struct ast *parent, struct ast *child)
{
    if (!parent)
        return child;
    parent->children = realloc(parent->children, nb_children + 1);
    parent->children[nb_children] = child;
    parent->nb_children++;
    return parent;
}

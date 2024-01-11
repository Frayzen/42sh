#include "ast_utils.h"

#include <stddef.h>
#include <stdlib.h>

struct ast *add_child(struct ast *src_ast, struct ast *new_ast)
{
    if (!src_ast)
        return new_ast;
    src_ast->children = realloc(src_ast->children, nb_children + 1);
    src_ast->children[nb_children] = new_ast;
    src_ast->nb_children++;
    return src_ast;
}

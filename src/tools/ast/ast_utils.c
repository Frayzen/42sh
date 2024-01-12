#include "ast_utils.h"

#include <stdio.h>
#include <stdlib.h>

struct ast *add_child(struct ast *parent, struct ast *child)
{
    if (!parent)
        return child;
    parent->children = realloc(parent->children,
                               sizeof(struct ast) * (parent->nb_children + 1));
    if (!parent->children)
    {
        // TODO handler error
        // TODO clear parent & child
        return NULL;
    }
    parent->children[parent->nb_children] = child;
    parent->nb_children++;
    return parent;
}

void pretty_print_ast_help(struct ast *ast_root, int depth, bool is_last_child,
                           bool last_of_first)
{
    if (!ast_root)
        return;
    for (int i = 0; i < depth - 1; i++)
    {
        if (last_of_first)
            printf("   ");
        else
            printf("║  ");
    }
    if (depth)
    {
        if (is_last_child)
            printf("╚══");
        else
            printf("╠══");
    }
    printf("%s\n", ast_root->token->value);
    for (int i = 0; i < ast_root->nb_children; i++)
    {
        if (i == ast_root->nb_children - 1)
        {
            if (!depth)
                last_of_first = true;
            is_last_child = true;
        }
        else
            is_last_child = false;
        pretty_print_ast_help(ast_root->children[i], depth + 1, is_last_child,
                              last_of_first);
    }
}

void pretty_print_ast(struct ast *ast)
{
    if (!ast)
    {
        printf("\nNULL ast\n");
        return;
    }
    printf("\n");
    pretty_print_ast_help(ast, 0, true, false);
    printf("\n");
}

#include "./pretty_printer.h"

#include <stdio.h>

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
    printf("\n");
    pretty_print_ast_help(ast, 0, true, false);
    printf("\n");
}

int main(void)
{
    struct token *ifToken = init_token("if");
    struct token *conditionToken = init_token("true");
    struct token *thenToken = init_token("then");
    struct token *bodyToken = init_token("echo");
    struct token *semicolonToken = init_token(";");
    struct ast *ifNode = init_ast(ifToken);
    struct ast *conditionNode = init_ast(conditionToken);
    struct ast *thenNode = init_ast(thenToken);
    struct ast *bodyNode = init_ast(bodyToken);
    struct ast *semicolonNode = init_ast(semicolonToken);
    ifNode->nb_children = 2;
    ifNode->children = malloc(ifNode->nb_children * sizeof(struct ast *));
    ifNode->children[0] = conditionNode;
    ifNode->children[1] = thenNode;
    thenNode->nb_children = 2;
    thenNode->children = malloc(thenNode->nb_children * sizeof(struct ast *));
    thenNode->children[0] = bodyNode;
    thenNode->children[1] = semicolonNode;
    pretty_print_ast(ifNode);
    destroy_ast(ifNode);
}

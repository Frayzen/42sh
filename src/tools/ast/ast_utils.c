#include "exit/error_handler.h"
#include "tools/ast/ast.h"

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_utils.h"

char *g_ast_types[] = {
    [AST_COMMAND] = "CMD", [AST_LIST] = "LST",  [AST_TOKEN] = "",
    [AST_IF] = "IF",       [AST_REDIR] = "RDR", [AST_NEGATE] = "NOT",
    [AST_PIPE] = "PIPE",   [AST_WHILE] = "WHL", [AST_UNTIL] = "UTL",
};

struct ast *add_child(struct ast *parent, struct ast *child)
{
    if (!parent)
        return child;
    parent->children = realloc(parent->children,
                               sizeof(struct ast) * (parent->nb_children + 1));
    if (!parent->children)
    {
        exit_gracefully(ADD_CHILD_ERROR);
    }
    parent->children[parent->nb_children] = child;
    parent->nb_children++;
    return parent;
}

int node_to_str(char *buf, struct ast *ast_root)
{
    if (ast_root == NULL)
    {
        strcpy(buf, "[NULL]");
        return 6;
    }
    if (ast_root->type == AST_TOKEN)
    {
        strcpy(buf, ast_root->token->value);
        return strlen(ast_root->token->value);
    }
    strcpy(buf, g_ast_types[ast_root->type]);
    return strlen(g_ast_types[ast_root->type]);
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
    static char buf[1024] = { 0 };
    node_to_str(buf, ast_root);
    printf("%s$\n", buf);
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
    pretty_print_ast_help(ast, 0, true, false);
}

void ast_to_str_rec(struct ast *ast, char *buf, size_t *id)
{
    *id += node_to_str(buf + *id, ast);
    buf[*id] = '\0';
    if (!ast || ast->nb_children == 0)
        return;
    buf[*id] = '{';
    (*id)++;
    ast_to_str_rec(ast->children[0], buf, id);
    for (int i = 1; i < ast->nb_children; i++)
    {
        buf[*id] = ',';
        (*id)++;
        ast_to_str_rec(ast->children[i], buf, id);
    }
    buf[*id] = '}';
    *id = *id + 1;
    buf[*id] = '\0';
}

char *ast_to_str(struct ast *ast)
{
    static char buf[4096] = { 0 };
    buf[0] = '\0';
    size_t i = 0;
    ast_to_str_rec(ast, buf, &i);
    return buf;
}

#include <stdlib.h>

#include "parser/tools/gr_utils.h"
#include "tools/ast/ast.h"

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

#include "ast_utils.h"

char *g_ast_types[] = {
    [AST_CMD] = "CMD",   [AST_LIST] = "LST",  [AST_IF] = "IF",
    [AST_PIPE] = "PIPE", [AST_WHILE] = "WHL", [AST_UNTIL] = "UTL",
    [AST_SH] = "SH",     [AST_ASS] = "ASS",   [AST_FOR] = "FOR",
};

int node_to_str(char *buf, struct ast *ast_root)
{
    if (!ast_root)
    {
        strcpy(buf, "[NULL]");
        return 6;
    }
    int len = strlen(g_ast_types[ast_root->type]);
    strcpy(buf, g_ast_types[ast_root->type]);
    return len;
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
    struct ast **children = get_children(ast_root);
    if (!children)
        return;
    int i = 0;
    while (children[i])
    {
        struct ast *child = children[i++];
        if (child)
        {
            if (!depth)
                last_of_first = true;
            is_last_child = true;
        }
        else
            is_last_child = false;
        pretty_print_ast_help(child, depth + 1, is_last_child, last_of_first);
    }
    free(children);
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
#define START
void ast_to_str_rec(struct ast *ast, char *buf, size_t *id)
{
    *id += node_to_str(buf + *id, ast);
    struct ast **children = get_children(ast);
    if (children)
        return;
    buf[(*id)++] = '{';
    int i = 0;
    while (children[i])
        ast_to_str_rec(children[i++], buf, id);
    buf[*id++] = '}';
    buf[++*id] = '\0';
    free(children);
}

char *ast_to_str(struct ast *ast)
{
    static char buf[4096] = { 0 };
    buf[0] = '\0';
    size_t i = 0;
    ast_to_str_rec(ast, buf, &i);
    return buf;
}

int write_buf(char *buffer, char *str, int i)
{
    int size = strlen(str);
    memcpy(buffer + i, str, size);
    return i + size;
}

void debug_pretty_print(struct ast *ast)
{
    /* printf("%s\n", ast_to_str(ast)); */
    pretty_print_ast(ast);
}

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser/tools/gr_utils.h"
#include "pretty_print/pretty_print.h"
#include "tools/ast/ast.h"

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

#include "ast_utils.h"

char *g_ast_types[] = {
    [AST_CMD] = "CMD",       [AST_LIST] = "LST",  [AST_IF] = "IF",
    [AST_PIPE] = "PIPE",     [AST_WHILE] = "WHL", [AST_UNTIL] = "UTL",
    [AST_SH] = "SH",         [AST_ASS] = "ASS",   [AST_FOR] = "FOR",
    [AST_AND_OR] = "AND_OR", [AST_CASE] = "CASE", [AST_FUNCT] = "FCT",
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

#define START
void ast_to_str_rec(struct ast *ast, char *buf, size_t *id)
{
    *id += node_to_str(buf + *id, ast);
    struct ast **children = get_children(ast);
    if (!children)
        return;
    buf[(*id)++] = '{';
    int i = 0;
    while (children[i])
        ast_to_str_rec(children[i++], buf, id);
    buf[(*id)++] = '}';
    buf[*id] = '\0';
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
    printf("%s\n", ast_to_str(ast));
    pretty_print_ast(ast);
}

struct ast *swap_ast(struct ast *new_ast)
{
    static struct ast *ast = NULL;
    struct ast *old_ast = ast;
    if (new_ast)
    {
        old_ast = ast;
        ast = new_ast;
    }
    return old_ast;
}

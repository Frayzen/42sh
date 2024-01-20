#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser/tools/gr_utils.h"
#include "tools/ast/ast.h"

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

#include "ast_utils.h"

char *g_ast_types[] = {
    [AST_CMD] = "CMD",       [AST_LIST] = "LST",  [AST_IF] = "IF",
    [AST_PIPE] = "PIPE",     [AST_WHILE] = "WHL", [AST_UNTIL] = "UTL",
    [AST_SH] = "SH",         [AST_ASS] = "ASS",   [AST_FOR] = "FOR",
    [AST_AND_OR] = "AND_OR",
};

void pretty_print_ast_help(struct ast *ast, enum ast_type type, int depth,
                           bool is_last_child, bool last_of_first);

void align(int depth, bool is_last_child, bool last_of_first)
{
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
}

void print_ast_list(struct ast_list *ast, int depth, bool is_last_child,
                    bool last_of_first)
{
    printf("LST\n");
    struct ast **children = get_children(AST(ast));
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
        pretty_print_ast_help(child, AST(child)->type, depth + 2, is_last_child,
                              last_of_first);
    }
    free(children);
}

void print_ast_pipe(struct ast_pipe *ast, int depth, bool last_of_first)
{
    printf("PIPE\n");
    depth++;
    struct ast **children = get_children(AST(ast));
    if (!children)
        return;
    int i = 0;
    while (children[i])
    {
        struct ast *child = children[i++];
        pretty_print_ast_help(child, child->type, depth + 1, false,
                              last_of_first);
    }
    free(children);
    char *neg = ast->negated ? "neg = true" : "neg = false";
    align(depth + 1, true, last_of_first);
    printf("%s\n", neg);
}

void print_ast_if(struct ast_if *ast, int depth, bool last_of_first)
{
    printf("IF\n");
    pretty_print_ast_help(AST(&ast->cond), AST_LIST, depth + 1, false,
                          last_of_first);
    pretty_print_ast_help(AST(&ast->then), AST_LIST, depth + 1, ast->fallback,
                          last_of_first);
    if (ast->fallback)
    {
        pretty_print_ast_help(ast->fallback, ast->fallback->type, depth + 1,
                              true, last_of_first);
    }
}

void print_ast_loop(struct ast_loop *ast, int depth, bool last_of_first)
{
    printf("LOOP\n");
    pretty_print_ast_help(AST(&ast->cond), AST_LIST, depth + 1, false,
                          last_of_first);
    pretty_print_ast_help(AST(&ast->exec), AST_LIST, depth + 1, true,
                          last_of_first);
}

void print_ast_cmd(struct ast_cmd *ast, int depth, bool last_of_first)
{
    printf("CMD\n");
    depth++;
    if (!ast->argc)
        return;
    align(depth, false, last_of_first);
    printf("argv\n");
    depth++;
    align(depth, false, last_of_first);
    printf("%s\n", ast->argv[0]);
    for (int i = 1; i < ast->argc; i++)
    {
        if (i == ast->argc - 1)
            align(depth, true, last_of_first);
        else
            align(depth, false, last_of_first);
        printf("%s\n", ast->argv[i]);
    }
    depth--;
    align(depth, true, last_of_first);
    bool is_builtin = ast->type <= ECHO && ast->type >= T_TRUE;
    printf("builtin %s\n", is_builtin ? "true" : "false");
    return;
}

void print_ast_sh(struct ast_sh *ast, int depth, bool last_of_first)
{
    printf("SH\n");
    align(depth, true, last_of_first);
    pretty_print_ast_help(ast->sh_cmd, AST(ast->sh_cmd)->type, depth + 1, true,
                          last_of_first);
}

void print_ast_for(struct ast_for *ast, int depth, bool last_of_first)
{
    printf("FOR\n");
    align(depth, false, last_of_first);
    if (!ast->item_list)
    {
        pretty_print_ast_help(AST(AST_LIST(ast)), AST_LIST, depth + 1, true,
                              last_of_first);
        return;
    }
    pretty_print_ast_help(AST(AST_LIST(ast)), AST_LIST, depth + 1, false,
                          last_of_first);
}

void print_ast_and_or(struct ast_and_or *ast, int depth, bool last_of_first)
{
    printf("AND_OR\n");
    if (ast->types)
    {
        align(depth + 1, false, last_of_first);
        printf("and_or\n");
        int i = 0;
        while (ast->types[i])
        {
            align(depth + 2, !ast->types[i + 1], last_of_first);
            printf("%s\n", ast->types[i] == AND ? "and" : "or");
            i++;
        }
    }
    pretty_print_ast_help(AST(AST_LIST(ast)), AST_LIST, depth + 1, true,
                          last_of_first);
}

void pretty_print_ast_help(struct ast *ast, enum ast_type type, int depth,
                           bool is_last_child, bool last_of_first)
{
    align(depth, is_last_child, last_of_first);
    switch (type)
    {
    case AST_LIST:
        print_ast_list(AST_LIST(ast), depth, is_last_child, last_of_first);
        break;
    case AST_PIPE:
        print_ast_pipe(AST_PIPE(ast), depth, last_of_first);
        break;
    case AST_IF:
        print_ast_if(AST_IF(ast), depth, last_of_first);
        break;
    case AST_WHILE:
    case AST_UNTIL:
        print_ast_loop(AST_LOOP(ast), depth, last_of_first);
        break;
    case AST_CMD:
        print_ast_cmd(AST_CMD(ast), depth, last_of_first);
        break;
    case AST_SH:
        print_ast_sh(AST_SH(ast), depth, last_of_first);
        break;
    case AST_FOR:
        print_ast_for(AST_FOR(ast), depth, last_of_first);
        break;
    case AST_AND_OR:
        print_ast_and_or(AST_AND_OR(ast), depth, last_of_first);
        break;
    default:
        // Should not happend
        assert(false);
        break;
    }
}

void pretty_print_ast(struct ast *ast)
{
    if (!ast)
    {
        printf("\nNULL AST\n");
        return;
    }
    pretty_print_ast_help(ast, AST(ast)->type, 0, true, false);
}

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

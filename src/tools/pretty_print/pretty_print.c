#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser/tools/gr_utils.h"
#include "tools/ast/ast.h"

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

#include "tools/pretty_print/pretty_print.h"

void pretty_print_ast_help(struct ast *ast, enum ast_type type, int depth,
                           bool is_last_child, bool *closed_nod);

void align(int depth, bool is_last_child, bool *closed_nod)
{
    if (closed_nod)
        closed_nod[depth - 1] = is_last_child;
    for (int i = 0; i < depth - 1; i++)
        printf("%s", closed_nod[i] ? "   " : "║  ");
    if (depth)
        printf("%s", is_last_child ? "╚══" : "╠══");
}
void print_ast_list(struct ast_list *ast, int depth, bool *closed_nod)
{
    printf("LST\n");
    struct ast **children = get_children(AST(ast));
    if (!children)
        return;
    int i = 0;
    while (children[i])
    {
        struct ast *child = children[i++];
        pretty_print_ast_help(child, AST(child)->type, depth + 1, !children[i],
                              closed_nod);
    }
    free(children);
}
void print_ast_pipe(struct ast_pipe *ast, int depth, bool *closed_nod)
{
    printf("PIPE\n");
    struct ast **children = get_children(AST(ast));
    if (!children)
        return;
    char *neg = ast->negated ? "neg = true" : "neg = false";
    align(++depth, false, closed_nod);
    printf("%s\n", neg);
    int i = 0;
    while (children[i])
    {
        struct ast *child = children[i++];
        pretty_print_ast_help(child, child->type, depth, !children[i],
                              closed_nod);
    }
    free(children);
}
void print_ast_if(struct ast_if *ast, int depth, bool *closed_nod)
{
    printf("IF\n");
    pretty_print_ast_help(AST(&ast->cond), AST_LIST, depth + 1, false,
                          closed_nod);
    pretty_print_ast_help(AST(&ast->then), AST_LIST, depth + 1, !ast->fallback,
                          closed_nod);
    if (ast->fallback)
    {
        pretty_print_ast_help(ast->fallback, ast->fallback->type, depth + 1,
                              true, closed_nod);
    }
}
void print_ast_loop(struct ast_loop *ast, int depth, bool *closed_nod)
{
    printf("LOOP\n");
    pretty_print_ast_help(AST(&ast->cond), AST_LIST, depth + 1, false,
                          closed_nod);
    pretty_print_ast_help(AST(&ast->exec), AST_LIST, depth + 1, true,
                          closed_nod);
}
void print_ast_cmd(struct ast_cmd *ast, int depth, bool *closed_nod)
{
    printf("CMD\n");
    depth++;
    if (!ast->args_expansion.size)
        return;
    struct expandable *current = ast->args_expansion.head;
    for (size_t i = 0; i < ast->args_expansion.size; i++)
    {
        align(depth, i == ast->args_expansion.size - 1, closed_nod);
        printf("%s\n", current->content);
        current = current->next;
    }
    depth--;
    return;
}
void print_ast_sh(struct ast_sh *ast, int depth, bool *closed_nod)
{
    printf("SH\n");
    align(depth, true, closed_nod);
    pretty_print_ast_help(ast->sh_cmd, AST(ast->sh_cmd)->type, depth + 1, true,
                          closed_nod);
}
void print_ast_for(struct ast_for *ast, int depth, bool *closed_nod)
{
    printf("FOR\n");
    depth++;
    if (!ast->item_list)
    {
        pretty_print_ast_help(AST(AST_LIST(ast)), AST_LIST, depth, true,
                              closed_nod);
        return;
    }
    pretty_print_ast_help(AST(AST_LIST(ast)), AST_LIST, depth, false,
                          closed_nod);
}
void print_ast_and_or(struct ast_and_or *ast, int depth, bool *closed_nod)
{
    printf("AND_OR\n");
    if (ast->types)
    {
        align(depth, false, closed_nod);
        printf("and_or\n");
        int i = 0;
        while (ast->types[i])
        {
            align(depth + 2, !ast->types[i + 1], closed_nod);
            printf("%s\n", ast->types[i] == AND ? "and" : "or");
            i++;
        }
    }
    pretty_print_ast_help(AST(AST_LIST(ast)), AST_LIST, depth + 1, true,
                          closed_nod);
}
bool get_max_depth(int depth)
{
    static int max_depth = 0;
    if (depth <= max_depth)
        return false;
    max_depth = depth;
    return true;
}
void pretty_print_ast_help(struct ast *ast, enum ast_type type, int depth,
                           bool is_last_child, bool *closed_nod)
{
    align(depth, is_last_child, closed_nod);
    if (get_max_depth(0) && depth)
    {
        closed_nod = realloc(closed_nod, (depth) * sizeof(bool));
        closed_nod[depth - 1] = false;
    }
    switch (type)
    {
    case AST_LIST:
        print_ast_list(AST_LIST(ast), depth, closed_nod);
        break;
    case AST_PIPE:
        print_ast_pipe(AST_PIPE(ast), depth, closed_nod);
        break;
    case AST_IF:
        print_ast_if(AST_IF(ast), depth, closed_nod);
        break;
    case AST_WHILE:
    case AST_UNTIL:
        print_ast_loop(AST_LOOP(ast), depth, closed_nod);
        break;
    case AST_CMD:
        print_ast_cmd(AST_CMD(ast), depth, closed_nod);
        break;
    case AST_SH:
        print_ast_sh(AST_SH(ast), depth, closed_nod);
        break;
    case AST_FOR:
        print_ast_for(AST_FOR(ast), depth, closed_nod);
        break;
    case AST_AND_OR:
        print_ast_and_or(AST_AND_OR(ast), depth, closed_nod);
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
    bool *closed_nod = calloc(1, sizeof(bool));
    pretty_print_ast_help(ast, AST(ast)->type, 0, true, closed_nod);
    free(closed_nod);
}

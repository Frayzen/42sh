#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "parser/command/expansion.h"
#include "parser/tools/gr_utils.h"
#include "tools/ast/ast.h"
#include "tools/str/string.h"

#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>

#include "tools/pretty_print/pretty_print.h"

// Setting colors
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN "\033[0;36m"
// Reset the color
#define RESET "\033[0m"

void pretty_print_ast_help(struct ast *ast, enum ast_type type, int depth,
                           bool is_last_child);

// Aligns the nod according to its depth
void align(int depth, bool is_last_child)
{
    for (int i = 0; i < depth - 1; i++)
        printf("   ");
    if (depth)
        printf("%s", is_last_child ? "╚══" : "╠══");
}

// Prints the nod and child of the ast of type AST_LIST
static void print_ast_list(struct ast_list *ast, int depth)
{
    printf(BLUE "LST\n" RESET);
    struct ast **children = get_children(AST(ast));
    if (!children)
        return;
    int i = 0;
    while (children[i])
    {
        struct ast *child = children[i++];
        pretty_print_ast_help(child, AST(child)->type, depth + 1, !children[i]);
    }
    free(children);
}

static void print_ast_subshell(struct ast_subshell *sub_shell,
                               int depth) //, bool *closed_nod)
{
    printf(YELLOW "SUB SHELL\n" RESET);
    struct ast **children = get_children(AST(sub_shell));
    if (!children)
        return;
    int i = 0;
    while (children[i])
    {
        struct ast *child = children[i++];
        pretty_print_ast_help(child, AST(child)->type, depth + 1, !children[i]);
        // closed_nod);
    }
    free(children);
}

// Prints the nod and child of the ast of type AST_PIPE
static void print_ast_pipe(struct ast_pipe *ast, int depth)
{
    printf(YELLOW "PIPE\n" RESET);
    struct ast **children = get_children(AST(ast));
    if (!children)
        return;
    char *neg = ast->negated ? "neg = true" : "neg = false";
    align(++depth, false);
    printf("%s\n", neg);
    int i = 0;
    while (children[i])
    {
        struct ast *child = children[i++];
        pretty_print_ast_help(child, child->type, depth, !children[i]);
    }
    free(children);
}

// Prints the nod and child of the ast of type AST_IF
static void print_ast_if(struct ast_if *ast, int depth)
{
    printf(GREEN "IF\n" RESET);
    pretty_print_ast_help(AST(&ast->cond), AST_LIST, depth + 1, false);

    pretty_print_ast_help(AST(&ast->then), AST_LIST, depth + 1, !ast->fallback);

    if (ast->fallback)
    {
        pretty_print_ast_help(ast->fallback, ast->fallback->type, depth + 1,
                              true);
    }
}

// Prints the nod and child of the ast of type AST_LOOP
void print_ast_loop(struct ast_loop *ast, int depth)
{
    printf(PURPLE "LOOP\n" RESET);
    pretty_print_ast_help(AST(&ast->cond), AST_LIST, depth + 1, false);

    pretty_print_ast_help(AST(&ast->exec), AST_LIST, depth + 1, true);
}

// Prints the nod and child of the ast of type AST_CMD
static void print_ast_cmd(struct ast_cmd *ast, int depth)
{
    printf(RED "CMD\n" RESET);
    depth++;
    if (!ast->args_expansion.size)
        return;
    struct expandable *current = ast->args_expansion.head;
    for (size_t i = 0; i < ast->args_expansion.size; i++)
    {
        align(depth, i == ast->args_expansion.size - 1);
        printf("%s\n", current->content);
        current = current->next;
    }
    depth--;
    return;
}

// Prints the nod and child of the ast of type AST_SH
static void print_ast_sh(struct ast_sh *ast, int depth)
{
    printf(RED "SH\n" RESET);
    pretty_print_ast_help(ast->sh_cmd, AST(ast->sh_cmd)->type, depth + 1, true);
}

// Prints the nod and child of the ast of type AST_FOR
static void print_ast_for(struct ast_for *ast, int depth)
{
    printf(CYAN "FOR\n" RESET);
    pretty_print_ast_help(AST(AST_LIST(ast)), AST_LIST, depth, false);
}

// Prints the nod and child of the ast of type AST_AND_OR
static void print_ast_and_or(struct ast_and_or *ast, int depth)
{
    printf(YELLOW "AND_OR\n" RESET);
    if (ast->types)
    {
        align(depth, false);
        printf("and_or\n");
        int i = 0;
        while (ast->types[i])
        {
            align(depth + 2, !ast->types[i + 1]);
            printf("%s\n", ast->types[i] == AND ? "and" : "or");
            i++;
        }
    }
    pretty_print_ast_help(AST(AST_LIST(ast)), AST_LIST, depth + 1, true);
}

void pretty_print_exapandable(struct expandable *exp)
{
    switch (exp->type)
    {
    case STR_LITTERAL:
        printf("%s\n", exp->content);
        break;
    case QUOTED_STR:
        printf("\"%s\"\n", exp->content);
        break;
    case QUOTED_VAR:
        printf("\"$%s\"\n", exp->content);
        break;
    case UNQUOTED_VAR:
        printf("$%s\n", exp->content);
        break;
    }
}

static void print_ast_case(struct ast_case *ast, int depth)
{
    printf(GREEN "CASE\n" RESET);
    depth++;
    align(depth, false);
    printf("name=");
    pretty_print_exapandable(ast->name.head);
    align(depth, !ast->nb_cond);
    printf("nb_cond=%d\n", ast->nb_cond);
    for (int i = 0; i < ast->nb_cond; i++)
    {
        int j = 0;
        while (ast->list_cond[i][j])
        {
            align(depth, i != ast->nb_cond + 1);
            pretty_print_exapandable(ast->list_cond[i][j]->head);
            j++;
        }
        pretty_print_ast_help(AST(ast->cmds[i]), AST_LIST, depth + 1, true);
    }
    depth--;
}

void pretty_print_ast_help(struct ast *ast, enum ast_type type, int depth,
                           bool is_last_child)
{
    align(depth, is_last_child);
    switch (type)
    {
    case AST_LIST:
        print_ast_list(AST_LIST(ast), depth);
        break;
    case AST_PIPE:
        print_ast_pipe(AST_PIPE(ast), depth);
        break;
    case AST_IF:
        print_ast_if(AST_IF(ast), depth);
        break;
    case AST_WHILE:
    case AST_UNTIL:
        print_ast_loop(AST_LOOP(ast), depth);
        break;
    case AST_CMD:
        print_ast_cmd(AST_CMD(ast), depth);
        break;
    case AST_SH:
        print_ast_sh(AST_SH(ast), depth);
        break;
    case AST_FOR:
        print_ast_for(AST_FOR(ast), depth);
        break;
    case AST_SUBSHELL:
        print_ast_subshell(AST_SUBSHELL(ast), depth);
        break;
    case AST_AND_OR:
        print_ast_and_or(AST_AND_OR(ast), depth);
        break;
    case AST_CASE:
        print_ast_case(AST_CASE(ast), depth);
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
    pretty_print_ast_help(ast, AST(ast)->type, 0, true);
}

#define _POSIX_C_SOURCE 200809L
#include <criterion/criterion.h>
#include <criterion/internal/test.h>
#include <string.h>

#include "exec/redirs/redirs.h"
#include "io_backend/backend_saver.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/token/token.h"

TestSuite(Redirs);

extern void build_redir(struct ast *ast, struct redir *redir);

struct ast *create_ast_token(char *c)
{
    struct exp_str str = {
        .value = strdup(c),
        .size = strlen(c),
    };
    return init_ast(AST_TOKEN, init_token(&str));
}

Test(Redirs, redir_basic)
{
    struct ast *ast = init_ast(AST_REDIR, NULL);
    add_child(ast, create_ast_token("1"));
    add_child(ast, create_ast_token(">"));
    add_child(ast, create_ast_token("test"));
    struct redir redir;
    build_redir(ast, &redir);
    cr_expect_eq(redir.dir, LEFT_TO_RIGHT);
    cr_expect_str_eq(redir.left, "1");
    cr_expect_str_eq(redir.right, "test");
    cr_expect_eq(redir.dup_io, false);
    cr_expect_eq(redir.append, false);
    destroy_ast(ast);
}

Test(Redirs, redir_left)
{
    struct ast *ast = init_ast(AST_REDIR, NULL);
    add_child(ast, create_ast_token("1"));
    io_push("<");
    add_child(ast, create_ast_token("<"));
    io_pop();
    add_child(ast, create_ast_token("3"));
    struct redir redir;
    build_redir(ast, &redir);
    cr_expect_eq(redir.dup_io, false);
    cr_expect_eq(redir.dir, RIGHT_TO_LEFT);
    cr_expect_str_eq(redir.left, "1");
    cr_expect_str_eq(redir.right, "3");
    cr_expect_eq(redir.append, false);
    destroy_ast(ast);
}

Test(Redirs, redir_append)
{
    struct ast *ast = init_ast(AST_REDIR, NULL);
    add_child(ast, create_ast_token("1"));
    io_push(">");
    add_child(ast, create_ast_token(">>"));
    io_pop();
    add_child(ast, create_ast_token("3"));
    struct redir redir;
    build_redir(ast, &redir);
    cr_expect_eq(redir.dup_io, false);
    cr_expect_eq(redir.dir, LEFT_TO_RIGHT);
    cr_expect_str_eq(redir.left, "1");
    cr_expect_str_eq(redir.right, "3");
    cr_expect_eq(redir.append, true);
    destroy_ast(ast);
}

Test(Redirs, redir_dup_1)
{
    struct ast *ast = init_ast(AST_REDIR, NULL);
    add_child(ast, create_ast_token("1"));
    io_push(">");
    add_child(ast, create_ast_token(">&"));
    io_pop();
    add_child(ast, create_ast_token("2"));
    struct redir redir;
    build_redir(ast, &redir);
    cr_expect_eq(redir.dup_io, true);
    cr_expect_eq(redir.dir, LEFT_TO_RIGHT);
    cr_expect_str_eq(redir.left, "1");
    cr_expect_str_eq(redir.right, "2");
    cr_expect_eq(redir.append, false);
    destroy_ast(ast);
}

Test(Redirs, redir_dup_2)
{
    struct ast *ast = init_ast(AST_REDIR, NULL);
    add_child(ast, create_ast_token("1"));
    add_child(ast, create_ast_token("<&"));
    add_child(ast, create_ast_token("next"));
    struct redir redir;
    build_redir(ast, &redir);
    cr_expect_eq(redir.dup_io, false);
    cr_expect_eq(redir.dir, RIGHT_TO_LEFT);
    cr_expect_str_eq(redir.left, "1");
    cr_expect_str_eq(redir.right, "next");
    cr_expect_eq(redir.append, false);
    destroy_ast(ast);
}

Test(Redirs, redir_bot_way)
{
    struct ast *ast = init_ast(AST_REDIR, NULL);
    add_child(ast, create_ast_token("1"));
    io_push(">");
    add_child(ast, create_ast_token("<>"));
    io_pop();
    add_child(ast, create_ast_token("2"));
    struct redir redir;
    build_redir(ast, &redir);
    cr_expect_eq(redir.dup_io, false);
    cr_expect_eq(redir.dir, BOTH_WAY);
    cr_expect_str_eq(redir.left, "1");
    cr_expect_str_eq(redir.right, "2");
    cr_expect_eq(redir.append, false);
    destroy_ast(ast);
}

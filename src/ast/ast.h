#ifndef AST_H
#define AST_H
#include "../token/token.h"

struct ast
{
    struct token *token;
    struct ast *left;
    struct ast *right;
};

struct ast *init_ast(struct token *token);
void destroy_ast(struct ast *ast);
void rec_destroy_ast(struct ast *ast);
void print_ast(struct ast *ast);

#endif // !AST_H

#ifndef AST_H
#define AST_H
#include "../token/token.h"

struct ast
{
    struct token *token;
    struct ast **children;
    int nb_children;
};

struct ast *init_ast(struct token *token);
void destroy_ast(struct ast *ast);

#endif // !AST_H

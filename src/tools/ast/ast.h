#ifndef AST_H
#define AST_H
#include "tools/token/token.h"

struct ast
{
    struct token *token;
    int nb_children;
    struct ast **children;
};

/***
 * Create a ast node structure with no children
 * @param token the token associated to the ast node
 * return malloced ast structure
 */
struct ast *init_ast(struct token *token);

/***
 * Recurisvely free an ast structure
 * @param ast the ast to free
 */
void destroy_ast(struct ast *ast);

#endif // !AST_H

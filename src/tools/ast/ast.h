#ifndef AST_H
#define AST_H
#include "tools/token/token.h"

enum ast_type
{
    AST_TOKEN,
    AST_COMMAND,
    AST_LIST,
    AST_IF,
    AST_REDIR
};

struct ast
{
    enum ast_type type;
    struct token *token;
    int nb_children;
    struct ast **children;
};

/***
 * Create a ast node structure with no children
 * @param ast_type the type of the associated ast
 * @param token the token associated to the ast node, NULL if not AST_TOKEN
 * return malloced ast structure
 */
struct ast *init_ast(enum ast_type, struct token *token);

/***
 * Recurisvely free an ast structure
 * @param ast the ast to free
 */
void destroy_ast(struct ast *ast);

#endif // !AST_H

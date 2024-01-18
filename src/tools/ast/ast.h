#ifndef AST_H
#define AST_H
#include "tools/token/token.h"

#define AST_ROOT (set_ast_root(NULL))

enum ast_type
{
    AST_TOKEN,
    AST_COMMAND,
    AST_LIST,
    AST_IF,
    AST_REDIR,
    AST_ASS,
    AST_NEGATE,
    AST_PIPE,
};

struct ast
{
    enum ast_type type;
    int nb_children;
    struct ast **children;
    struct token *token;
};

/***
 * Set or gets the pointer to the ast root
 * @param pointer to the ast to set as the root if not NULL
 * @return the pointer to the ast root
 */
struct ast **set_ast_root(struct ast **ast);

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

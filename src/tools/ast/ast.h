#ifndef AST_H
#define AST_H
#include <fcntl.h>
#include <stdbool.h>

#include "tools/token/token.h"

#define AST_ROOT (set_ast_root(NULL))

enum ast_type
{
    AST_CMD,
    AST_SH,
    AST_LIST,
    AST_IF,
    AST_ASS,
    AST_PIPE,
    AST_WHILE,
    AST_UNTIL,
    // NOT USED
    AST_AND,
    AST_OR
};

#define AST(Base) ((struct ast *)(Base))
struct ast
{
    enum ast_type type;
};

#define AST_LIST(Base) ((struct ast_list *)(Base))
struct ast_list
{
    struct ast *base;
    int nb_children;
    struct ast **children;
};

#define AST_PIPE(Base) ((struct ast_pipe *)(Base))
struct ast_pipe
{
    struct ast_list list;
    bool negated;
};

#define AST_IF(Base) ((struct ast_if *)(Base))
struct ast_if
{
    struct ast base;
    struct ast_list *cond;
    struct ast_list *then;
    // might be ast_if OR ast_list
    struct ast *fallback;
};

#define AST_LOOP(Base) ((struct ast_loop *)(Base))
struct ast_loop
{
    struct ast base;
    struct ast_list *cond;
    struct ast_list *exec;
};

#define AST_REDIR(Base) ((struct ast_redir *)(Base))
struct ast_redir
{
    struct ast base;
    struct redirection **redirs;
    int redir_nb;
};

#define AST_CMD(Base) ((struct ast_cmd *)(Base))
struct ast_cmd
{
    struct ast redirs;
    enum token_type type;
    char **argv;
    int argc;
};

#define AST_SH(Base) ((struct ast_sh *)(Base))
struct ast_sh
{
    struct ast_redir redirs;
    struct ast *sh_cmd;
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
 * @return malloced ast structure
 */
void *init_ast(enum ast_type type);

/***
 * Recurisvely free an ast structure
 * @param ast the ast to free
 */
void destroy_ast(void *ast);

#endif // !AST_H

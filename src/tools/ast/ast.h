#ifndef AST_H
#define AST_H
#include <fcntl.h>
#include <stdbool.h>

#include "parser/command/expansion.h"
#include "tools/assignment/assignment.h"
#include "tools/token/token.h"

#define AST_ROOT (swap_ast_root(NULL))

enum ast_type
{
    // Leave it to zero for nested lists to have their type set
    AST_LIST = 0,
    AST_CMD,
    AST_SH,
    AST_IF,
    AST_PIPE,
    AST_WHILE,
    AST_UNTIL,
    AST_FOR,
    AST_AND_OR,
    AST_SUBSHELL,
    AST_FUNCT,
    // NOT USED
    AST_ASS
};

#define AST(Base) ((struct ast *)(Base))
struct ast
{
    enum ast_type type;
};

#define AST_LIST(Base) ((struct ast_list *)(Base))
struct ast_list
{
    struct ast base;
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
    struct ast_list cond;
    struct ast_list then;
    // might be ast_if (ELIF) OR ast_list(ELSE) OR NULL
    struct ast *fallback;
};

#define AST_LOOP(Base) ((struct ast_loop *)(Base))
struct ast_loop
{
    struct ast base;
    struct ast_list cond;
    struct ast_list exec;
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
    struct ast_redir redirs;
    struct expansion args_expansion;
    struct assignment_list assignment_list;
};

#define AST_SH(Base) ((struct ast_sh *)(Base))
struct ast_sh
{
    struct ast_redir redirs;
    struct ast *sh_cmd;
};

#define AST_FOR(Base) ((struct ast_for *)(Base))
struct ast_for
{
    struct ast_list cmds;
    char *name;
    struct expansion exp;
};

#define AST_SUBSHELL(Base) ((struct ast_subshell *)(Base))
struct ast_subshell
{
    struct ast_list cmds;
};

// BEGIN AND OR
#define AST_AND_OR(Base) ((struct ast_and_or *)(Base))
struct ast_and_or
{
    struct ast_list list;
    enum token_type *types;
};
// END AND OR

#define AST_FUNCT(Base) ((struct ast_funct *)(Base))
struct ast_funct
{
    struct ast_list list;
    char *name;
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

/***
 * Swap old and new ast
 * @param new_ast, the ast to swap
 * @return the old ast
 */
struct ast **swap_ast_root(struct ast **new_ast);

#endif // !AST_H

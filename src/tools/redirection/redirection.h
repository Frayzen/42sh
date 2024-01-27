#ifndef REDIRECTION_H
#define REDIRECTION_H
#include "tools/ast/ast.h"
#define DBG_OUT 666
#define DBG_ERR 667

// THE OUT ARE ALWAYS EVEN AND THE IN ARE ODDS
enum redir_type
{
    RT_MASK_IN = 1,

    RT_REDIR_OUT = 2, // > and >|
    RT_REDIR_IN = 3, // <
    RT_APPEND_OUT = 4, // >>
    RT_READ_WRITE = 5, // <>
    RT_MASK_DUP = 8,
    RT_DUP_IN_FD = 9, // <&
    RT_DUP_OUT_FD = 10, // >&
};

struct redirection
{
    int io_number;
    enum redir_type type;
    struct expansion exp;
};

/***
 * Append a redirection allocated to the ast_redir
 * @param ast the ast_redir
 * @param redir the redirection
 */
void append_redir(struct ast_redir *ast, struct redirection *redir);

/***
 * Free all redirections and the list from the redirection ast
 * @param ast the ast
 */
void destroy_redir(struct ast_redir *ast);

/***
 * Return the type of redirection based on the chevron char
 * @param chevron the chevron as a null terminated string
 * @return the type
 */
enum redir_type get_redir_type(char *chevron);

/***
 * Setup the redirections for an ast
 * @param ast the ast
 * @return the file descriptors to be saved
 * @see close_redirs
 */
int *setup_redirs(struct ast_redir *ast);
/***
 * Close the file descriptor previously saved (@see setup_redirs)
 * @param fds the saved file descriptors
 */
void close_redirs(int *saved);

/***
 * Setup the file descriptor to debug
 */
void setup_debug_fds(void);

/***
 * Close the file descriptor to debug
 */
void clean_debug_fds(void);

#endif /* !REDIRECTION_H */

#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "tools/ast/ast.h"

enum redir_type
{
    RT_REDIR_OUT, // > and >|
    RT_REDIR_IN, // <
    RT_DUP_OUT_FD, // >&
    RT_DUP_IN_FD, // <&
    RT_APPEND_OUT, // >>
    RT_READ_WRITE, // <>
};

struct redirection
{
    int io_number;
    enum redir_type type;
    char *to;
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
#endif /* !REDIRECTION_H */

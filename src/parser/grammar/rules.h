#ifndef RULES_H
#define RULES_H

#include "tools/ast/ast.h"

/***
 * Macros to decide if need to goto error
 * @param test the condition to check
 * @param function the function to goto
 */
#define CHECK_GOTO(test, function)                                             \
    if (test)                                                                  \
    {                                                                          \
        goto function;                                                         \
    }

enum status
{
    OK,
    ERROR,
};

typedef enum status (*gr_rule)(struct ast **ast);

/***
 * Grammar rule for element
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */
enum status gr_input(struct ast **ast);

/***
 * Grammar rule for element
 * @param cmd: the command tree to be updated
 * @return enum that tells the status
 */
enum status gr_element(struct ast_cmd *cmd);

/***
 * Grammar rule for simple_command
 * @param list: the ast list to be updated
 * @return enum that tells the status
 */

enum status gr_simple_command(struct ast_list *list);

/***
 * Grammar rule for command
 * @param ast: the pipe to be updated
 * @return enum that tells the status
 */

enum status gr_command(struct ast_pipe *pipe);

/***
 * Grammar rule for pipeline
 * @param list the list to be updated
 * @return enum that tells the status
 */

enum status gr_pipeline(struct ast_list *list);

/***
 * Grammar rule for and_or
 * @param ast: the list to be updated
 * @return enum that tells the status
 */
enum status gr_and_or(struct ast_list *list);

/***
 * Grammar rule for list
 * @param new_list the pointer that will be filled by the new list
 * @return enum that tells the status
 */
enum status gr_list(struct ast **new_list);

/***
 * Grammar rule for compound lists
 * @param ast: the tree to be set to the read compound list
 * @return enum that tells the status
 */

enum status gr_compound_list(struct ast_list **new_compound);

/***
 * Grammar rule for elif
 * @param ast: the if to be updated
 * @return enum that tells the status
 */

enum status gr_else(struct ast_if *ast);

/***
 * Grammar rule for if
 * @param list: the list to be updated
 * @return enum that tells the status
 */

enum status gr_if(struct ast_list *ast);

/***
 * Grammar rule for shell comand
 * @param list: the list to be updated
 * @return enum that tells the status
 */
enum status gr_shell_cmd(struct ast_list *list);

/***
 * Grammar rule for redirection
 * @param redir: the ast redir to be updated
 * @return enum that tells the status
 */
enum status gr_redir(struct ast_redir *redir);

/***
 * Grammar rule for prefix
 * @param cmd: the cmd ast to be updated
 * @return enum that tells the status
 */
enum status gr_prefix(struct ast_cmd *cmd);

/***
 * Grammar rule for while
 * @param list: the list to be updated
 * @return enum that tells the status
 */
enum status gr_while(struct ast_list *ast);

/***
 * Grammar rule for until
 * @param list: the list to be updated
 * @return enum that tells the status
 */
enum status gr_until(struct ast_list *ast);

/***
 * Grammar rule for for
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */
enum status gr_for(struct ast_list *ast);

#endif /* !RULES_H */

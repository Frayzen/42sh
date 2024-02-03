#ifndef RULES_H
#define RULES_H

#include "tools/ast/ast.h"

enum status
{
    OK,
    ERROR,
    NO_MATCH,
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
 * Grammar rule for coumpound_list, fill the provided ast (needs to exists)
 * /!\ If error is returned, the list_ast need to be freed by the calling
 * function
 * @param list_ast the ast to be filled
 * @return enum that tells the status
 */
enum status gr_compound_list(struct ast_list *list_ast);

/***
 * Grammar rule for elif
 * @param ast: the if to be updated
 * @return enum that tells the status
 */

enum status gr_else(struct ast_if *ast);

/***
 * Grammar rule for if
 * @param sh: the shell ast to be updated
 * @return enum that tells the status
 */

enum status gr_if(struct ast_sh *sh);

/***
 * Grammar rule for shell comand
 * @param list: the ast to be updated
 * @return enum that tells the status
 */
enum status gr_shell_cmd(struct ast *list);

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
 * @param sh: the shell ast to be updated
 * @return enum that tells the status
 */
enum status gr_while(struct ast_sh *sh);

/***
 * Grammar rule for until
 * @param sh: the shell ast to be updated
 * @return enum that tells the status
 */
enum status gr_until(struct ast_sh *sh);

/***
 * Grammar rule for for
 * @param sh: the shell ast to be updated
 * @return enum that tells the status
 */
enum status gr_for(struct ast_sh *sh);

/***
 * Grammar rule for and_or
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */
enum status gr_and_or(struct ast_list *list);

/***
 * Grammar rule for case
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */
enum status gr_case(struct ast_sh *sh);

/***
 * Grammar rule for case clause
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */
enum status gr_case_clause(struct ast_case *ast);

/***
 * Grammar rule for case item
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */
enum status gr_case_item(struct ast_case *ast);

/***
 * Grammar rule for functions
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */
enum status gr_function(struct ast_list *list);

#endif /* !RULES_H */

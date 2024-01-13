#ifndef GRULES_H
#define GRULES_H

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

/***
 * Grammar rule for element
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */
enum status gr_input(struct ast **ast);

/***
 * Grammar rule for element
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */
enum status gr_element(struct ast **ast);

/***
 * Grammar rule for simple_command
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */

enum status gr_simple_command(struct ast **ast);

/***
 * Grammar rule for command
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */

enum status gr_command(struct ast **ast);

/***
 * Grammar rule for pipeline
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */

enum status gr_pipeline(struct ast **ast);

/***
 * Grammar rule for and_or
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */

enum status gr_and_or(struct ast **ast);

/***
 * Grammar rule for list
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */

enum status gr_list(struct ast **ast);

/***
 * Grammar rule for compound lists
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */

enum status gr_compound_list(struct ast **ast);

/***
 * Grammar rule for elif
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */

enum status gr_else(struct ast **ast);

/***
 * Grammar rule for if
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */

enum status gr_if(struct ast **ast);

/***
 * Grammar rule for shell comand
 * @param ast: the tree to be updated
 * @return enum that tells the status
 */
enum status gr_shell_cmd(struct ast **ast);

#endif /* !GRULES_H */

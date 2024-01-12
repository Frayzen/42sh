#ifndef GRULES_H
#define GRULES_H

#include "tools/ast/ast.h"

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

#endif /* !GRULES_H */

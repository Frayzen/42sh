#ifndef ENV_H
#define ENV_H
#include <stdlib.h>

#include "tools/token/token.h"
extern char **environ;
// need to add the default value in the get_env_flag function for each new flag

struct env
{
    int print;
    int verbose;
    char **environ;
};

/***
 * get the environment flag structure
 * @return a pointer to the environment flag structure
 */
struct env *get_env_flag(void);

/***
 * executes the verbose flag
 * @param token the token to print
 */
void verbose(struct token *token);
#endif /* ENV_H */

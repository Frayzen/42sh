#ifndef ENV_H
#define ENV_H

#include <sys/wait.h>

#include "tools/token/token.h"
extern char **environ;
// need to add the default value in the get_env_flag function for each new flag

#define DBG_PIPE(...)                                                          \
    if (get_env_flag()->debug_pipe)                                            \
        printf(__VA_ARGS__);
#define VERBOSE(...)                                                           \
    if (get_env_flag()->verbose)                                               \
        printf(__VA_ARGS__);

#define STDIN (get_env_flag()->fds[0])
#define STDOUT (get_env_flag()->fds[1])

struct env
{
    char **environ;
    bool null_received;
    bool pretty_print;
    bool verbose;
    bool debug_pipe;
    bool is_interactive;
    int fds[2];
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

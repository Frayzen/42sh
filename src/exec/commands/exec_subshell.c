#include <sys/wait.h>
#include <unistd.h>

#include "execs.h"
#include "execs_cmd.h"

int exec_subshell(struct ast_subshell *ast)
{
    pid_t pid = fork();
    if (pid == -1)
        return 1;
    if (pid == 0)
        _exit(exec_list(AST_LIST(ast)));
    int returncode = 0;
    int retval = 0;
    waitpid(pid, &returncode, 0);
    if (WIFEXITED(returncode))
        retval = WEXITSTATUS(returncode);
    return retval;
}

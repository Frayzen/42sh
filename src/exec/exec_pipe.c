#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#include "env/env.h"
#include "execs.h"
#include "tools/ast/ast.h"

// return the result of the function and update out
int exec_piped(struct ast *ast, int in, int out)
{
    DBG_PIPE("Command %s in %d out %d\n", ast->children[0]->token->value, in,
             out);
    assert(ast->type == AST_COMMAND);
    struct sh_command cmd = { .root = ast,
                              .redirs_fds = { in, out, dup(STDOUT_FILENO) },
                              0 };
    int ret = 1;
    if (!build_command(&cmd))
        goto clean;
    ret = exec_sh_command(&cmd);
clean:
    close(cmd.redirs_fds[STDIN_FILENO]);
    close(cmd.redirs_fds[STDERR_FILENO]);
    return ret;
}

int exec_pipe(struct ast *ast)
{
    assert(ast->type == AST_PIPE && ast->nb_children != 0);
    int ret = 1;
    int last_read = dup(STDIN_FILENO);
    DBG_PIPE("Duplicate %d in %d\n", STDIN_FILENO, last_read);
    int i = 0;
    for (; i < ast->nb_children - 1; i++)
    {
        // p[0] is read and p[1] is write
        int p[2];
        if (pipe(p) == -1)
        {
            perror("Error while piping:");
            return 1;
        }
        DBG_PIPE("Create pipe [write]%d => %d[read]\n", p[1], p[0]);
        exec_piped(ast->children[i], last_read, p[1]);
        close(last_read);
        last_read = p[0];
        close(p[1]);
    }
    ret = exec_piped(ast->children[i], last_read, STDOUT_FILENO);
    fflush(NULL);
    return ret;
}

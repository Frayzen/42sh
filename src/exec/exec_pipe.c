#include "env/env.h"
#include "execs.h"
#include "tools/ast/ast.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

//return the result of the function and update out
int exec_piped(struct ast *ast, int new_stdin, int* out)
{
    assert(ast->type == AST_COMMAND);
    struct sh_command cmd = {
        .root = ast,
        .redirs_fds = {new_stdin, dup(STDOUT_FILENO), dup(STDOUT_FILENO)},
        0
    };
    int ret = 1;
    if (!build_command(&cmd))
        goto clean;
    ret = exec_sh_command(&cmd);
clean:
    close(cmd.redirs_fds[STDIN_FILENO]);
    *out = cmd.redirs_fds[STDOUT_FILENO];
    close(cmd.redirs_fds[STDERR_FILENO]);
    return ret;
}

int exec_pipe(struct ast *ast)
{
    assert(ast->type == AST_PIPE && ast->nb_children != 0);
    int ret = 1;
    int last = dup(STDIN_FILENO);
    for (int i = 0; i < ast->nb_children; i++) {
        //p[0] is read and p[1] is write
        int p[2] = {0};
        if (pipe(p) == -1)
        {
            perror("Error while piping:");
            return 1;
        }
        dup2(p[0], last);
        if (get_env_flag()->verbose)
            printf("Pipe %d -> %d\n", p[0], last);
        ret = exec_piped(ast->children[i], p[0], &last);     
    }
    dup2(STDOUT_FILENO, last);
    return ret;
}

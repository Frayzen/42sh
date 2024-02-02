#include "env/vars/specials.h"
#define _POSIX_C_SOURCE 200809L

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include "env/env.h"
#include "execs.h"
#include "execs_cmd.h"
#include "exit/error_handler.h"
#include "tools/ast/ast.h"

// either set the pid and return PID_SET or return the value
int exec_piped(struct ast *ast, int in, int out, int *pid)
{
    int ret;
    dup2(in, STDIN_FILENO);
    dup2(out, STDOUT_FILENO);
    DBG_PIPE("[PIPE] New stdin=%d | stdout=%d\n\n", in, out);
    switch (ast->type)
    {
    case AST_CMD:
        ret = exec_cmd(AST_CMD(ast), pid);
        break;
    case AST_SH:
        ret = exec_sh(AST_SH(ast));
        break;
    case AST_FUNCT:
        ret = exec_store_funct(AST_FUNCT(ast));
        break;
    default:
        print_error(PIPE_NOT_FOUND);
        ret = 1;
    }
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    return ret;
}

// return the return value of the pid
// if pid is negative, handle the builtin
int wait_for(int pid)
{
    if (pid == -1)
        return 1;
    int returncode;
    waitpid(pid, &returncode, 0);
    int retval = 0;
    if (WIFEXITED(returncode))
        retval = WEXITSTATUS(returncode);
    if (retval == 127)
        print_error(FORK_ERROR);
    fflush(NULL);
    return retval;
}

int exec_pipe(struct ast_pipe *ast)
{
    DBG_PIPE("[PIPE] START -----\n");
    assert(AST(ast)->type == AST_PIPE);
    struct ast_list *list = AST_LIST(ast);
    // Save
    int saved[2] = { dup(STDIN_FILENO), dup(STDOUT_FILENO) };
    int last_read = dup(STDIN_FILENO);
    DBG_PIPE("[PIPE] Duplicate STDIN in %d\n", last_read);
    int i = 0;
    int *pids = calloc(list->nb_children, sizeof(int));
    for (; i < list->nb_children - 1; i++)
    {
        // p[0] is read and p[1] is write
        int p[2];
        if (pipe(p) == -1)
        {
            perror("Error while piping:");
            return 1;
        }
        DBG_PIPE("[PIPE] Create pipe [write]%d => %d[read]\n", p[1], p[0]);
        pids[i] = exec_piped(list->children[i], last_read, p[1], &pids[i]);
        close(last_read);
        close(p[1]);
        last_read = p[0];
    }
    int retval =
        exec_piped(list->children[i], last_read, saved[1], &pids[i]);
    close(last_read);
    // Wait for all of them
    for (int j = 0; j < list->nb_children - 1; j++)
        wait_for(pids[j]);
    // Set return value
    if (retval == PID_SET)
        retval = wait_for(pids[i]);
    set_ret_val(retval);
    free(pids);
    if (ast->negated)
        retval = !retval;
    // restore
    dup2(saved[0], STDIN_FILENO);
    dup2(saved[1], STDOUT_FILENO);
    DBG_PIPE("[PIPE] ------ END\n\n");
    return retval;
}

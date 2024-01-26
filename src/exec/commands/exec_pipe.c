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
    int oldin = STDOUT;
    int oldout = STDIN;
    STDIN = in;
    STDOUT = out;
    DBG_PIPE("[PIPE] New stdin=%d | stdout=%d\n\n", STDIN, STDOUT);
    switch (ast->type)
    {
    case AST_CMD:
        ret = exec_cmd(AST_CMD(ast), pid);
        break;
    case AST_IF:
        ret = exec_condition(AST_IF(ast));
        break;
    case AST_WHILE:
    case AST_UNTIL:
        ret = exec_loop(AST_LOOP(ast));
        break;
    case AST_FOR:
        ret = exec_for(AST_FOR(ast));
        break;
    case AST_FUNCT:
        ret = exec_store_funct(AST_FUNCT(ast));
        break;
    default:
        print_error(PIPE_NOT_FOUND);
        ret = 1;
    }
    STDIN = oldin;
    STDOUT = oldout;
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
    int last_read = dup(STDIN);
    DBG_PIPE("[PIPE] Duplicate %d in %d\n", STDIN, last_read);
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
        exec_piped(list->children[i], last_read, STDOUT_FILENO, &pids[i]);
    close(last_read);
    // Wait for all of them
    for (int j = 0; j < list->nb_children - 1; j++)
        wait_for(pids[j]);
    // Set return value
    if (retval == PID_SET)
        retval = wait_for(pids[i]);
    free(pids);
    if (ast->negated)
        retval = !retval;
    DBG_PIPE("[PIPE] ------ END\n\n");
    return retval;
}

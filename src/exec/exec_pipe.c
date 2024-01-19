#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "env/env.h"
#include "execs.h"
#include "exit/error_handler.h"
#include "tools/ast/ast.h"

int exec_piped(struct ast *ast, int in, int out)
{
    int ret;
    int oldin = STDOUT;
    int oldout = STDIN;
    STDIN = in;
    STDOUT = out;
    DBG_PIPE("[PIPE] New stdin=%d | stdout=%d\n\n", STDIN, STDOUT);
    switch (ast->type)
    {
    case AST_COMMAND:
        ret = exec_command(ast);
        break;
    case AST_IF:
        ret = exec_condition(ast);
        break;
    case AST_WHILE:
        ret = exec_while(ast);
        break;
    case AST_UNTIL:
        ret = exec_until(ast);
        break;

    default:
        print_error(PIPE_NOT_FOUND);
        ret = 1;
    }
    STDIN = oldin;
    STDOUT = oldout;
    return ret;
}

int exec_pipe(struct ast *ast)
{
    assert(ast->type == AST_PIPE && ast->nb_children != 0);
    int ret = 1;
    int last_read = dup(STDIN);
    DBG_PIPE("[PIPE] Duplicate STDIN(%d) in %d\n", STDIN, last_read);
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
        fcntl(last_read, F_SETFD, FD_CLOEXEC);
        fcntl(p[1], F_SETFD, FD_CLOEXEC);
        DBG_PIPE("[PIPE] Create pipe [write]%d => %d[read]\n", p[1], p[0]);
        exec_piped(ast->children[i], last_read, p[1]);
        DBG_PIPE("[PIPE] Close last read (%d) and write (%d)\n", last_read,
                 p[1]);
        close(last_read);
        close(p[1]);
        last_read = p[0];
    }
    ret = exec_piped(ast->children[i], last_read, STDOUT_FILENO);
    close(last_read);
    fflush(NULL);
    return ret;
}

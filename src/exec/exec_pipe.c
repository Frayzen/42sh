#define _POSIX_C_SOURCE 200809L
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
    case AST_CMD:
        ret = exec_command(AST_CMD(ast));
        break;
    case AST_IF:
        ret = exec_condition(AST_IF(ast));
        break;
    case AST_WHILE:
    case AST_UNTIL:
        ret = exec_loop(AST_LOOP(ast));
        break;
    case AST_FOR:
        // TODO exec_for
        ret = 0;
        break;
    default:
        print_error(PIPE_NOT_FOUND);
        ret = 1;
    }
    STDIN = oldin;
    STDOUT = oldout;
    return ret;
}

int exec_pipe(struct ast_pipe *ast)
{
    DBG_PIPE("[PIPE] START -----\n");
    assert(AST(ast)->type == AST_PIPE);
    struct ast_list *list = AST_LIST(ast);
    int ret = 1;
    int last_read = dup(STDIN);
    DBG_PIPE("[PIPE] Duplicate %d in %d\n", STDIN, last_read);
    int i = 0;
    for (; i < list->nb_children - 1; i++)
    {
        // p[0] is read and p[1] is write
        int p[2];
        if (pipe(p) == -1)
        {
            perror("Error while piping:");
            return 1;
        }
        DBG_PIPE("Create pipe [write]%d => %d[read]\n", p[1], p[0]);
        exec_piped(list->children[i], last_read, p[1]);
        close(last_read);
        close(p[1]);
        last_read = p[0];
    }
    ret = exec_piped(list->children[i], last_read, STDOUT_FILENO);
    fflush(NULL);
    if (ast->negated)
        ret = !ret;
    DBG_PIPE("[PIPE] ------ END\n\n");
    return ret;
}

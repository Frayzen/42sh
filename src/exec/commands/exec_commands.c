#include "env/vars/vars.h"
#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "env/env.h"
#include "exec/builtins/builtins.h"
#include "exec/commands/execs_cmd.h"
#include "exit/error_handler.h"
#include "parser/command/expander.h"
#include "tools/ast/ast.h"
#include "tools/redirection/redirection.h"

// Fork execute the binary and return pid in parent
int exec_prog(char **argv)
{
    int pid = fork();
    if (pid == -1)
    {
        print_error(FORK_ERROR);
        return 1;
    }
    if (pid == 0)
    {
        DBG_PIPE("Command %s fds are [IN] %d | [OUT] %d | [ERR] %d\n", argv[0],
                 STDIN, STDOUT, STDERR);
        // Apply the file descriptors before executing
        for (int i = 0; i < 3; i++)
        {
            dup2(FDS[i], i);
            // Do not close if STDOUT = STOUD_FILENO
            if (i != FDS[i])
                close(FDS[i]);
        }
        execvp(argv[0], argv);
        exit(127);
    }
    return pid;
}

void destroy_argv(char **argv)
{
    int i = 0;
    while (argv[i])
        free(argv[i++]);
    free(argv);
}

void apply_assignments(struct assignment_list *asslist)
{
    for (unsigned int i = 0; i < asslist->size; i++)
    {
        struct assignment *ass = asslist->ass_list[i];
        char *val = NULL;
        assert(expand_next(ass->exp.head, &val) == NULL);
        assert(val != NULL);
        ass->prev = assign_var(ass->name, val);
        free(val);
    }
}

void revert_assignments(struct assignment_list *asslist)
{
    for (unsigned int i = 0; i < asslist->size; i++)
    {
        struct assignment *ass = asslist->ass_list[i];
        free(assign_var(ass->name, ass->prev));
    }
}

int exec_cmd(struct ast_cmd *ast, int *pid)
{
    assert(ast && AST(ast)->type == AST_CMD);
    int *fds = setup_redirs(AST_REDIR(ast));
    if (!fds)
        return 1;
    int ret = 2;
    char **argv = expand(&ast->args_expansion);
    apply_assignments(&ast->assignment_list);

    *pid = PID_SET;
    if (argv[0])
    {
        if (!strcmp(argv[0], "echo"))
        {
            builtin_echo(argv);
            ret = 0;
        }
        else if (!strcmp(argv[0], "exit"))
            builtin_exit(argv);
        else if (!strcmp(argv[0], "true"))
            ret = 0;
        else if (!strcmp(argv[0], "false"))
            ret = 1;
        else
        {
            *pid = exec_prog(argv);
            ret = -1;
        }
        revert_assignments(&ast->assignment_list);
    }
    destroy_argv(argv);
    close_redirs(fds);
    return ret;
}

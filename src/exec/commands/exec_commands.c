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

#include "commands/execs_cmd.h"
#include "env/env.h"
#include "exit/error_handler.h"
#include "parser/command/args.h"
#include "tools/ast/ast.h"
#include "tools/redirection/redirection.h"

void print_echo(char **argv, int i, bool interpret_bslash, bool print_nline)
{
    DBG_PIPE("Echo command [OUT] %d\n", STDOUT);
    for (; argv[i]; i++)
    {
        const char *content = argv[i];
        int id = 0;
        while (content[id])
        {
            if (content[id] == '\\' && interpret_bslash)
            {
                id++;
                switch (content[id])
                {
                case 'n':
                    dprintf(STDOUT, "\n");
                    break;
                case '\\':
                    dprintf(STDOUT, "\\");
                    break;
                case 't':
                    dprintf(STDOUT, "\t");
                    break;
                default:
                    continue;
                }
            }
            else
                dprintf(STDOUT, "%c", content[id]);
            id++;
        }
        if (argv[i + 1])
            dprintf(STDOUT, " ");
    }
    if (print_nline)
        dprintf(STDOUT, "\n");
}

/***
 * sets the options according to the content sequence
 * returns true if the sequence sets options and false otherwise
 * ie: if content does not start with '-' or contains a char that is not 'n' 'e'
 * or 'E'
 */
bool set_option_echo(const char *content, bool *interpret_bslash,
                     bool *print_nline)
{
    bool init_bslash = *interpret_bslash;
    bool init_print = *print_nline;
    if (content[0] != '-')
        return false;
    for (int i = 1; content[i] != '\0'; i++)
    {
        switch (content[i])
        {
        case 'n':
            *print_nline = false;
            break;
        case 'e':
            *interpret_bslash = true;
            break;
        case 'E':
            *interpret_bslash = false;
            break;
        default:
            *print_nline = init_print;
            *interpret_bslash = init_bslash;
            return false;
        }
    }
    return true;
}

void exec_echo(char **argv)
{
    int i = 1;
    bool print_nline = true;
    bool interpret_bslash = false;
    while (argv[i])
    {
        const char *content = argv[i];
        if (!set_option_echo(content, &interpret_bslash, &print_nline))
            break;
        i++;
    }
    print_echo(argv, i, interpret_bslash, print_nline);
    fflush(NULL);
}

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

int exec_cmd(struct ast_cmd *ast, int *pid)
{
    assert(ast && AST(ast)->type == AST_CMD);
    int *fds = setup_redirs(AST_REDIR(ast));

    if (!fds)
        return 1;
    int ret = 2;
    char **argv = build_argv(&ast->arglist);
    print_ass_list(&ast->ass_list);
    assign_vars(ast->ass_list);

    *pid = PID_SET;
    if (!strcmp(argv[0], "echo"))
    {
        exec_echo(argv);
        ret = 0;
    }
    else if (!strcmp(argv[0], "true"))
        ret = 0;
    else if (!strcmp(argv[0], "false"))
        ret = 1;
    else
    {
        *pid = exec_prog(argv);
        ret = -1;
    }
    destroy_argv(argv);
    close_redirs(fds);
    return ret;
}

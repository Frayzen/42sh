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
#include "execs.h"
#include "exit/error_handler.h"
#include "tools/ast/ast.h"
#include "tools/redirection/redirection.h"
#include "tools/token/token.h"

void print_echo(struct ast_cmd *cmd, int i, bool interpret_bslash,
                bool print_nline)
{
    DBG_PIPE("Echo command [OUT] %d\n", STDOUT);
    for (; i < cmd->argc; i++)
    {
        const char *content = cmd->argv[i];
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
        if (cmd->argc - 1 != i)
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

int exec_echo(struct ast_cmd *cmd)
{
    assert(cmd && cmd->type == ECHO);
    int i = 1;
    bool print_nline = true;
    bool interpret_bslash = false;
    while (i < cmd->argc)
    {
        const char *content = cmd->argv[i];
        if (!set_option_echo(content, &interpret_bslash, &print_nline))
            break;
        i++;
    }
    print_echo(cmd, i, interpret_bslash, print_nline);
    fflush(NULL);
    close_redirs(cmd);
    return 0;
}

int external_bin(struct ast_cmd *cmd)
{
    int pid = fork();
    if (pid == -1)
    {
        print_error(FORK_ERROR);
        return 1;
    }
    if (pid == 0)
    {
        DBG_PIPE("Command %s fds are [IN] %d | [OUT] %d | [ERR] %d\n",
                 cmd->argv[0], STDIN, STDOUT, STDERR);
        // Apply the file descriptors before executing
        for (int i = 0; i < 3; i++)
        {
            dup2(FDS[i], i);
            // Do not close if STDOUT = STOUD_FILENO
            if (i != FDS[i])
                close(FDS[i]);
        }
        dprintf(STDERR_FILENO, "This is an error\n");
        execvp(cmd->argv[0], cmd->argv);
        exit(127);
    }
    int returncode;
    waitpid(pid, &returncode, 0);
    int code = 0;
    if (WIFEXITED(returncode))
        code = WEXITSTATUS(returncode);
    if (code == 127)
        print_error(FORK_ERROR);
    fflush(NULL);
    return code;
}

int exec_command(struct ast_cmd *ast)
{
    assert(ast && AST(ast)->type == AST_CMD);
    assert(ast->argc != 0);
    int *fds = setup_redirs(AST_REDIR(ast));
    if (!fds)
        return 1;
    int ret = 1;
    switch (ast->type)
    {
    case ECHO:
        ret = exec_echo(ast);
        break;
    case T_TRUE:
        ret = 0;
        break;
    case T_FALSE:
        ret = 1;
        break;
    default:
        ret = external_bin(ast);
        break;
    }
    close_redirs(fds);
    return ret;
}

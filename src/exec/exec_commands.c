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
#include "tools/token/token.h"

void print_echo(struct ast_cmd *cmd, int i, bool interpret_bslash,
                bool print_nline)
{
    /* DBG_PIPE("Echo command [OUT] %d\n", cmd->redirs_fds[1]); */
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
    while (i < cmd->argc - 1)
    {
        const char *content = cmd->argv[i];
        if (!set_option_echo(content, &interpret_bslash, &print_nline))
            break;
        i++;
    }
    print_echo(cmd, i, interpret_bslash, print_nline);
    fflush(NULL);
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
        /* DBG_PIPE("Command %s fds are [IN] %d | [OUT] %d | [ERR] %d\n", */
        /*          cmd->argv[0], cmd->redirs_fds[0], cmd->redirs_fds[1], */
        /*          cmd->redirs_fds[2]); */
        /* for (int i = 0; i < 3; i++) */
        /*     dup2(cmd->redirs_fds[i], i); */
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
    fflush(stdout);
    return code;
}

int exec_sh_command(struct ast_cmd *ast)
{
    switch (ast->type)
    {
    case ECHO:
        return exec_echo(ast);
    case T_TRUE:
        return 0;
    case T_FALSE:
        return 1;
    default:
        return external_bin(ast);
    }
}

int exec_command(struct ast_cmd *ast)
{
    assert(ast && AST(ast)->type == AST_CMD);
    assert(ast->argc != 0);
    int ret = 1;
    ret = exec_sh_command(ast);
    return ret;
}

#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "env/env.h"
#include "exec/redirs/redirs.h"
#include "execs.h"
#include "exit/error_handler.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

void print_echo(struct sh_command *cmd, int i, bool interpret_bslash,
                bool print_nline)
{
    DBG_PIPE("Echo command [OUT] %d\n", cmd->redirs_fds[1]);
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
                    dprintf(cmd->redirs_fds[1], "\n");
                    break;
                case '\\':
                    dprintf(cmd->redirs_fds[1], "\\");
                    break;
                case 't':
                    dprintf(cmd->redirs_fds[1], "\t");
                    break;
                default:
                    continue;
                }
            }
            else
                dprintf(cmd->redirs_fds[1], "%c", content[id]);
            id++;
        }
        if (cmd->argc - 1 != i)
            dprintf(cmd->redirs_fds[1], " ");
    }
    if (print_nline)
        dprintf(cmd->redirs_fds[1], "\n");
}

int exec_echo(struct sh_command *cmd)
{
    struct ast *ast = cmd->root;
    assert(ast && ast->type == AST_COMMAND
           && ast->children[0]->token->type == ECHO);
    int i = 1;
    bool print_nline = true;
    bool interpret_bslash = false;
    while (i < cmd->argc - 1)
    {
        const char *content = cmd->argv[i];
        if (!strcmp(content, "-e"))
            interpret_bslash = true;
        else if (!strcmp(content, "-E"))
            interpret_bslash = false;
        else if (!strcmp(content, "-n"))
            print_nline = false;
        else
            break;
        i++;
    }
    print_echo(cmd, i, interpret_bslash, print_nline);
    fflush(NULL);
    return 0;
}

int external_bin(struct sh_command *cmd)
{
    int pid = fork();
    if (pid == 0)
    {
        DBG_PIPE("Command %s fds are [IN] %d | [OUT] %d | [ERR] %d\n",
                 cmd->argv[0], cmd->redirs_fds[0], cmd->redirs_fds[1],
                 cmd->redirs_fds[2]);
        for (int i = 0; i < 3; i++)
            dup2(cmd->redirs_fds[i], i);
        execvp(cmd->argv[0], cmd->argv);
        exit(127);
    }
    int returncode;
    waitpid(pid, &returncode, 0);
    int code = 0;
    if (WIFEXITED(returncode))
        code = WEXITSTATUS(returncode);
    if (code)
        print_error(FORK_ERROR);
    fflush(stdout);
    return code;
}

// true if everything is fine
bool build_command(struct sh_command *cmd)
{
    struct ast *ast = cmd->root;
    assert(ast && ast->type == AST_COMMAND);
    assert(ast->nb_children != 0);
    cmd->argv = calloc(ast->nb_children + 1, sizeof(char *));
    cmd->argc = 0;
    for (int i = 0; i < ast->nb_children; i++)
    {
        if (ast->children[i]->type == AST_REDIR)
        {
            if (!apply_redirection(cmd, ast->children[i]))
            {
                print_error(BAD_REDIRECTION);
                return false;
            }
        }
        else
            cmd->argv[cmd->argc++] = ast->children[i]->token->value;
    }
    return true;
}

int exec_sh_command(struct sh_command *command)
{
    struct token *token = command->root->children[0]->token;
    switch (token->type)
    {
    case ECHO:
        return exec_echo(command);
    case T_TRUE:
        return 0;
    case T_FALSE:
        return 1;
    default:
        return external_bin(command);
    }
}

int exec_command(struct ast *ast)
{
    assert(ast && ast->type == AST_COMMAND);
    assert(ast->nb_children != 0);
    struct sh_command command = { .root = ast,
                                  .redirs_fds = { STDIN, STDOUT, 2 },
                                  0 };
    int ret = 1;
    if (build_command(&command))
        ret = exec_sh_command(&command);
    free(command.argv);
    return ret;
}

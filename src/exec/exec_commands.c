#include <stdbool.h>
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

bool set_flag_echo(const char *content, bool *interpret_bslash,
                   bool *print_nline)
{
    bool init_bslash = *interpret_bslash;
    bool init_print = *print_nline;
    if (content[0] != '-')
        return false;
    for (int i = 1; content[i] != '\0'; i++)
    {
        if (content[i] == 'n')
            *print_nline = false;
        else if (content[i] == 'e')
            *interpret_bslash = true;
        else if (content[i] == 'E')
            *interpret_bslash = false;
        else
        {
            *print_nline = init_print;
            *interpret_bslash = init_bslash;
            return false;
        }
    }
    return true;
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
        if (!set_flag_echo(content, &interpret_bslash, &print_nline))
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
    fflush(stdout);
    return code;
}

int execute(struct sh_command *command)
{
    struct token *token = command->root->children[0]->token;
    int ret;
    switch (token->type)
    {
    case ECHO:
        return exec_echo(command);
    case T_TRUE:
        return 0;
    case T_FALSE:
        return 1;
    default:
        ret = external_bin(command);
        if (ret)
            print_error(FORK_ERROR);
        return ret;
    }
}

// true if everything is fine
bool build_command(struct sh_command *cmd)
{
    struct ast *ast = cmd->root;
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

int exec_command(struct ast *ast)
{
    assert(ast && ast->type == AST_COMMAND);
    assert(ast->nb_children != 0);
    struct sh_command command = { 0 };
    for (int i = 0; i < 3; i++)
        command.redirs_fds[i] = i;
    command.root = ast;
    if (!build_command(&command))
        return 1;
    int ret = execute(&command);
    free(command.argv);
    return ret;
}

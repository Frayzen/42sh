#include "tools/str/string.h"
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
        // printf("arg = %s\n", cmd->str[i]->value);
        // const char *content = cmd->str[i]->value;
        // printf("arg = %s\n", cmd->argv[i]);
        const char *content = cmd->str[i]->value;
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

void exec_echo(struct ast_cmd *cmd)
{
    assert(cmd && cmd->type == ECHO);
    int i = 1;
    bool print_nline = true;
    bool interpret_bslash = false;
    while (i < cmd->argc)
    {
        const char *content = cmd->str[i]->value;
        if (!set_option_echo(content, &interpret_bslash, &print_nline))
            break;
        i++;
    }
    print_echo(cmd, i, interpret_bslash, print_nline);
    fflush(NULL);
}

// this will include the expansion
char **expand_args(struct ast_cmd *cmd)
{
    char **args = calloc(1, sizeof(char *) * cmd->argc + 1);
    for (int i = 0; i < cmd->argc; i++)
        args[i] = cmd->str[i]->value;
    args[cmd->argc] = NULL;
    return args;
}

// Fork execute the binary and return pid in parent
int external_bin(struct ast_cmd *cmd)
{
    char **argv = expand_args(cmd);
    int pid = fork();
    if (pid == -1)
    {
        print_error(FORK_ERROR);
        return 1;
    }
    if (pid == 0)
    {
        DBG_PIPE("Command %s fds are [IN] %d | [OUT] %d | [ERR] %d\n",
                 argv[0], STDIN, STDOUT, STDERR);
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
    free(argv);
    return pid;
}

// Execute the builtin and return the return value
int exec_builtin(struct ast_cmd *ast)
{
    assert(ast && AST(ast)->type == AST_CMD);
    assert(ast->argc != 0 && ast->is_builtin);
    int *fds = setup_redirs(AST_REDIR(ast));

    if (!fds)
        return 1;
    int ret;
    switch (ast->type)
    {
    case ECHO:
        exec_echo(ast);
        ret = 0;
        break;
    case T_TRUE:
        ret = 0;
        break;
    case T_FALSE:
        ret = 1;
        break;
    default:
        // Should not happen
        assert(false);
        ret = 1;
        break;
    }
    close_redirs(fds);
    return ret;
}

// Execute the binary and return the pid
int exec_bin(struct ast_cmd *ast)
{
    assert(ast && AST(ast)->type == AST_CMD);
    assert(ast->argc != 0 && !ast->is_builtin);
    int *fds = setup_redirs(AST_REDIR(ast));
    if (!fds)
        return -1;
    int ret = external_bin(ast);
    close_redirs(fds);
    return ret;
}

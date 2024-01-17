#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "execs.h"
#include "exit/exit.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

void print_echo(struct ast *ast, int i, bool interpret_bslash)
{
    for (; i < ast->nb_children; i++)
    {
        const char *content = ast->children[i]->token->value;
        int id = 0;
        while (content[id])
        {
            if (content[id] == '\\' && interpret_bslash)
            {
                id++;
                switch (content[id])
                {
                case 'n':
                    printf("\n");
                    break;
                case '\\':
                    printf("\\");
                    break;
                case 't':
                    printf("\t");
                    break;
                default:
                    continue;
                }
            }
            else
                printf("%c", content[id]);
            id++;
        }
        if (ast->nb_children - 1 != i)
            printf(" ");
    }
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
    print_echo(ast, i, interpret_bslash);
    if (print_nline)
        printf("\n");
    fflush(stdout);
    return 0;
}

int external_bin(struct sh_command *cmd)
{
    int pid = fork();
    if (pid == 0)
    {
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

void apply_redirection(struct sh_command *cmd, struct ast *redir)
{
    assert(redir->type == AST_REDIR);
    int from = -1;
    int i = 0;
    struct token *token = redir->children[i++]->token;
    if (token->type == IO_NUMBER)
    {
        from = atoi(token->value);
        token = redir->children[i++]->token;
    }
    else {
        if (token->value[0] == '<')
            from = 1;
        if (token->value[0] == '>')
            from = 0;
    }
    assert(from != -1);
    bool is_io = token->value[1] == '&';
    bool append = token->value[1] == '>';
    bool both_way = token->value[1] == '<';
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

void build_command(struct sh_command *cmd)
{
    struct ast *ast = cmd->root;
    cmd->argv = calloc(ast->nb_children + 1, sizeof(char *));
    cmd->argc = 0;
    for (int i = 0; i < ast->nb_children; i++)
    {
        if (ast->children[i]->type == AST_REDIR)
            apply_redirection(cmd, ast->children[i]);
        else
            cmd->argv[cmd->argc++] = ast->children[i]->token->value;
    }
}



int exec_command(struct ast *ast)
{
    assert(ast && ast->type == AST_COMMAND);
    assert(ast->nb_children != 0);
    struct sh_command command = { 0 };
    command.root = ast;
    build_command(&command);
    int ret = execute(&command);
    free(command.argv);
    return ret;
}

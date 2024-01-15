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

int exec_echo(struct ast *ast)
{
    assert(ast && ast->type == AST_COMMAND
           && ast->children[0]->token->type == ECHO);
    int i = 1;
    bool print_nline = true;
    bool interpret_bslash = false;
    while (i < ast->nb_children - 1)
    {
        const char *content = ast->children[i]->token->value;
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

// this creates the char ** needed for the arguments of execvp
char **create_command(struct ast *ast)
{
    size_t size_array = 1;
    char **array_arg = calloc(size_array, sizeof(char *));
    for (int i = 0; i < ast->nb_children; i++)
    {
        array_arg[size_array - 1] = ast->children[i]->token->value;
        array_arg = realloc(array_arg, sizeof(char *) * size_array + 1);
        size_array++;
    }
    array_arg[size_array - 1] = NULL;
    return array_arg;
}

int external_bin(struct ast *ast)
{
    int pid = fork();
    if (pid == 0)
    {
        char **array_arg = create_command(ast);
        int resp = execvp(array_arg[0], array_arg);
        free(array_arg);
        if (resp) // false
        {
            exit_gracefully(EXECVP_FAILED);
        }
        else
        {
            int returncode;
            waitpid(pid, &returncode, 0);
            int code = 0;
            if (WIFEXITED(returncode))
                code = WEXITSTATUS(returncode);
            if (code == 1)
                return 1;
            return 0;
        }
    }
    fflush(stdout);
    return 0;
}

int exec_external_bin(struct ast *ast)
{
    int ret = external_bin(ast);
    if (ret)
        exit_gracefully(FORK_ERROR);
    return ret;
}

int exec_command(struct ast *ast)
{
    assert(ast && ast->type == AST_COMMAND);
    assert(ast->nb_children != 0);
    switch (ast->children[0]->token->type)
    {
    case ECHO:
        return exec_echo(ast);
    case T_TRUE:
        return 0;
    case T_FALSE:
        return 1;
    default:
        return exec_external_bin(ast);
    }
}

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

int exec_echo(struct ast *ast)
{
    if (ast->children[0]->token->type == ECHO)
    {
        for (int i = 1; i < ast->nb_children - 1; i++)
        {
            printf("%s ", ast->children[i]->token->value);
        }
        if (ast->nb_children > 1)
            printf("%s\n", ast->children[ast->nb_children - 1]->token->value);
        else
            printf("\n");
    }
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
            print_error(EXECVP_FAILED);
            return 1;
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
    return 0;
}

int exec_external_bin(struct ast *ast)
{
    int ret = external_bin(ast);
    if (ret)
        print_error(FORK_ERROR);
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

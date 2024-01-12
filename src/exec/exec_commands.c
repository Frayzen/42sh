#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "tools/ast/ast.h"
#include "tools/token/token.h"

#include <assert.h>
#include <stdio.h>

#include "execs.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

void exec_echo(struct ast *ast)
{
    if (ast->children[0]->token->type == ECHO)
    {
        for (int i = 1; i < ast->nb_children - 1; i++)
        {
            printf("%s ", ast->children[i]->token->value);
        }
        if (ast->nb_children > 1)
            printf("%s\n", ast->children[ast->nb_children - 1]->token->value);
    }
    fflush(stdout);
}

char **create_command(struct ast *ast)
{
    size_t size_array = 1;
    char **array_arg = calloc(size_array,sizeof(char *));
    for (int i = 0; i < ast->nb_children; i++)
    {
        array_arg[size_array - 1] = ast->children[i]->token->value;
        array_arg = realloc(array_arg, sizeof(char *) * size_array + 1);
        size_array++;
    }
    array_arg[size_array - 1] = NULL;
    return array_arg;
}

int basic_function(struct ast *ast)
{
    int pid = fork();
    if (pid == 0)
    {
        char **array_arg = create_command(ast);
        int resp  = execvp(array_arg[0], array_arg);
        if (resp)
        {
            //TODO handle errors
            return -1;
        }
        else
        {
            int returncode;
            waitpid(pid, &returncode, 0);
            int code = 0;
            if (WIFEXITED(returncode))
                code = WEXITSTATUS(returncode);
            if (code == -1)
                //TODO handle error
                return -1;
            return 0;
        }
    }
    return 0;
}

void exec_basic_function(struct ast *ast)
{
    if (basic_function(ast) == -1)
    {
        //TODO handle error
        return;
    }
}
void exec_command(struct ast *ast)
{
    assert(ast && ast->type == AST_COMMAND);
    assert(ast->nb_children != 0);
    switch (ast->children[0]->token->type)
    {
    case ECHO:
        exec_echo(ast);
        break;
    default:
        exec_basic_function(ast);
        break;
    }
}

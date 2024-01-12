#include <assert.h>
#include <stdio.h>

#include "execs.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

void exec_echo(struct ast *ast)
{
    assert(ast && ast->type == AST_COMMAND);
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

void exec_command(struct ast *ast)
{
    switch (ast->children[0]->token->type)
    {
    case ECHO:
        exec_echo(ast);
        break;
    default:
        break;
    }
}

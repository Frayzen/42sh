#include <stdio.h>
#include <string.h>

#include "stdlib.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"
void echo_function(struct ast *ast)
{
    if (!ast)
    {
        // TODO handle error
        return;
    }
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

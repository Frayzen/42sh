#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include "tools/ast/ast.h"
#include "tools/token/token.h"
void echo_function(struct ast *ast)
{
    if (ast->children[0]->token->type == ECHO)
    {
        for (int i = 1; i < ast->nb_children - 1; i++)
        {
            printf("%s ", ast->children[i]->token->value);
        }
        printf("%s\n", ast->children[ast->nb_children - 1]->token->value);
    }
}


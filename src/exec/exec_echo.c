#include <stdio.h>
#include <string.h>
#include "tools/ast/ast.h"
#include "tools/token/token.h"
void echo_function(struct ast *ast)
{
    if (ast->children[0]->token->type == ECHO)
    {
        for (int i = 1; i < ast->nb_children - 1; i++)
        {
            char *child_token = ast->children[i]->token->value;
            if (child_token[strlen(child_token - 1)] == ' ')
                printf("%s", ast->children[i]->token->value);
            else
                printf("%s ", ast->children[i]->token->value);
        }
        printf("%s\n", ast->children[ast->nb_children - 1]->token->value);
    }
}


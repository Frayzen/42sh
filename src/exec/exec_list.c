#include "execs.h"

void exec_list(struct ast *ast)
{
    for(int i = 0; i < ast->nb_children; i++)
    {
        exec_command(ast->children[i]);
    }
}

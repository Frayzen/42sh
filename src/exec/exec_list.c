#include <assert.h>

#include "execs.h"

void exec_list(struct ast *ast)
{
    assert(ast && ast->type == AST_LIST);
    for (int i = 0; i < ast->nb_children; i++)
    {
        exec_command(ast->children[i]);
    }
}

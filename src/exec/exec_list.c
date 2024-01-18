#include <assert.h>

#include "execs.h"
#include "exit/error_handler.h"
#include "tools/ast/ast.h"

int exec_list(struct ast *ast)
{
    assert(ast && ast->type == AST_LIST && ast->nb_children > 0);
    int ret = -1;
    for (int i = 0; i < ast->nb_children; i++)
    {
        struct ast *child = ast->children[i];
        switch (child->type)
        {
        case AST_NEGATE:
            ret = exec_negate(child);
            break;
        case AST_PIPE:
            ret = exec_pipe(child);
            break;
        default:
            exit_gracefully(LIST_NOT_FOUND);
            return 1;
        }
    }
    return ret;
}

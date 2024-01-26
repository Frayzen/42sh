#include <assert.h>

#include "commands/execs_cmd.h"
#include "execs.h"
#include "exit/error_handler.h"
#include "tools/ast/ast.h"

int exec_list(struct ast_list *ast)
{
    assert(AST(ast)->type == AST_LIST || AST(ast)->type == AST_FOR);
    assert(ast && ast->nb_children > 0);
    int ret = -1;
    for (int i = 0; i < ast->nb_children; i++)
    {
        struct ast *child = ast->children[i];
        switch (child->type)
        {
        case AST_PIPE:
            ret = exec_pipe(AST_PIPE(child));
            break;
        case AST_AND_OR:
            ret = exec_and_or(AST_AND_OR(child));
            break;
        case AST_FUNCT:
            ret = exec_store_funct(AST_FUNCT(child));
            break;
        default:
            exit_gracefully(LIST_NOT_FOUND);
            return 2;
        }
    }
    return ret;
}

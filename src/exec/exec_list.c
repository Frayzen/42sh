#include <assert.h>

#include "execs.h"
#include "exit/exit.h"
#include "tools/ast/ast.h"

int exec_list(struct ast *ast)
{
    assert(ast && ast->type == AST_LIST && ast->nb_children > 0);
    int ret = 1;
    for (int i = 0; i < ast->nb_children; i++)
        switch (ast->children[i]->type)
        {
        case AST_COMMAND:
            ret = exec_command(ast->children[i]);
            break;
        case AST_IF:
            ret = exec_condition(ast->children[i]);
            break;
        default:
            print_error(LIST_NOT_FOUND);
            return 1;
        }
    return ret;
}

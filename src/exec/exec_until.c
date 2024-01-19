#include <assert.h>

#include "execs.h"
#include "tools/ast/ast.h"

int exec_until(struct ast *ast)
{
    assert(ast && ast->type == AST_UNTIL && ast->nb_children == 2);
    int ret = exec_list(ast->children[0]);
    if (ret)
    {
        exec_list(ast->children[1]);
        exec_until(ast);
    }
    return 1;
}

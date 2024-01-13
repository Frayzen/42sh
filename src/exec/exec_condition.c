#include <assert.h>
#include <stdio.h>

#include "execs.h"
#include "tools/ast/ast.h"

int exec_condition(struct ast *ast)
{
    assert(ast && ast->type == AST_IF && ast->nb_children >= 2);
    if (!exec_list(ast->children[0]))
        return exec_list(ast->children[1]);
    if (ast->nb_children == 2)
        return 0;
    return exec_list(ast->children[2]);
}

#include <assert.h>
#include <stdio.h>

#include "execs.h"
#include "tools/ast/ast.h"

int exec_condition(struct ast *ast)
{
    assert(ast && ast->type == AST_IF && ast->nb_children >= 2);
    int ret = exec_list(ast->children[0]);
    printf("ret = %d\n", ret);
    if (!ret)
        return exec_list(ast->children[1]);
    if (ast->nb_children == 2)
        return 0;
    if (ast->children[2]->type == AST_IF)
        return exec_condition(ast->children[2]);
    return exec_list(ast->children[2]);
}

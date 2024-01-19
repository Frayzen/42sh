#include <assert.h>

#include "execs.h"
#include "tools/ast/ast.h"

int exec_condition(struct ast_if *ast)
{
    assert(ast && AST(ast)->type == AST_IF);
    assert(ast->then && ast->cond);
    if (!exec_list(ast->cond))
        return exec_list(ast->then);
    if (!ast->fallback)
        return 0;
    if (ast->fallback->type == AST_LIST)
        return exec_list(AST_LIST(ast->fallback));
    assert(ast->fallback->type == AST_IF);
    return exec_condition(AST_IF(ast->fallback));
}

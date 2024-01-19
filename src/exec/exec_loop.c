#include <assert.h>

#include "execs.h"
#include "tools/ast/ast.h"

int exec_loop(struct ast_loop *ast)
{
    assert(AST(ast)->type == AST_WHILE || AST(ast)->type == AST_UNTIL);
    assert(ast && ast->cond && ast->exec);
    bool negate = AST(ast)->type == AST_UNTIL;
    int ret = exec_list(ast->cond);
    while (negate ? !ret : ret)
    {
        exec_list(ast->exec);
        ret = exec_list(ast->cond);
    }
    return 0;
}

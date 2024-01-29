#include <assert.h>
#include <stdio.h>

#include "execs.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

int exec_loop(struct ast_loop *ast)
{
    assert(AST(ast)->type == AST_WHILE || AST(ast)->type == AST_UNTIL);
    get_nb_loop(NB_LOOPS + 1);
    bool negate = AST(ast)->type == AST_UNTIL;
    int ret = exec_list(&ast->cond);
    //! ret for 0
    while (negate ? ret : !ret)
    {
        if (CONTINUE == NB_LOOPS)
        {
            get_continue(-1);
            continue;
        }
        else if (BREAK == NB_LOOPS)
        {
            get_break(-1);
            break;
        }
        else if (CONTINUE || BREAK)
            break;
        exec_list(&ast->exec);
        ret = exec_list(&ast->cond);
    }
    get_nb_loop(NB_LOOPS - 1);
    return 0;
}

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "execs.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

int exec_loop(struct ast_loop *ast)
{
    assert(AST(ast)->type == AST_WHILE || AST(ast)->type == AST_UNTIL);
    set_nb_loop(NB_LOOPS + 1);
    bool negate = AST(ast)->type == AST_UNTIL;
    int ret = exec_list(&ast->cond);
    //! ret for 0
    while (negate ? ret : !ret)
    {
        if (CNTU_LAYER)
        {
            negate = false;
            if (IS_CUR_LOOP(CNTU_LAYER - 1))
            {
                set_continue(CNTU_LAYER - 1);
                continue;
            }
            break;
        }
        if (BREAK_LAYER)
        {
            negate = false;
            if (IS_CUR_LOOP(CNTU_LAYER - 1))
            {
                set_break(BREAK_LAYER - 1);
                continue;
            }
            break;
        }
        exec_list(&ast->exec);
        ret = exec_list(&ast->cond);
    }
    set_continue(CNTU_LAYER - 1);
    set_break(BREAK_LAYER - 1);
    set_nb_loop(NB_LOOPS - 1);
    return 0;
}

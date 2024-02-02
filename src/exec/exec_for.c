#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <env/env.h>
#include <unistd.h>

#include "env/vars/vars.h"
#include "execs.h"
#include "parser/command/expander.h"
#include "tools/ast/ast.h"

extern char **environ;

int exec_for(struct ast_for *ast)
{
    assert(ast && AST(ast)->type == AST_FOR);
    set_nb_loop(NB_LOOPS + 1);
    char **elems = expand(&ast->exp);
    int ret = 0;
    int i = 0;
    while (elems[i])
    {
        if (CNTU_LAYER)
        {
            set_continue(CNTU_LAYER - 1);
            if (IS_CUR_LOOP(CNTU_LAYER))
                continue;
            break;
        }
        if (BREAK_LAYER)
        {
            set_break(BREAK_LAYER - 1);
            if (IS_CUR_LOOP(BREAK_LAYER))
                continue;
            break;
        }
        assign_var(ast->name, elems[i]);
        ret = exec_list(AST_LIST(ast));
        i++;
    }
    set_nb_loop(NB_LOOPS - 1);
    destroy_expanded(elems);
    return ret;
}

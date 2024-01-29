#include <stdio.h>
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
    get_nb_loop(NB_LOOPS + 1);
    char **elems = expand(&ast->exp);
    int ret = 0;
    int i = 0;
    while (elems[i])
    {
        if (CONTINUE)
        {
            if (CONTINUE - 1)
            {
                get_continue(CONTINUE - 1);
                break;
            }
            else
            {
                get_continue(-1);
                continue;
            }
        }
        else if (BREAK)
        {
            if (BREAK - 1)
                get_break(BREAK - 1);
            else
                get_break(-1);
            break;
        }
        assign_var(ast->name, elems[i]);
        ret = exec_list(AST_LIST(ast));
        i++;
    }
    get_nb_loop(NB_LOOPS - 1);
    destroy_expanded(elems);
    return ret;
}

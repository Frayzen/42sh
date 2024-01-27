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
        if (CONTINUE == NB_LOOPS)
        {
            get_continue(-2);
            continue;
        }
        else if (BREAK == NB_LOOPS)
        {
            get_break(-2);
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

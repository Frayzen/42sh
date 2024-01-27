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
    char **elems = expand(&ast->exp);
    int ret = 0;
    int i = 0;
    while (elems[i])
    {
        assign_var(ast->name, elems[i]);
        ret = exec_list(AST_LIST(ast));
        i++;
    }
    destroy_expanded(elems);
    return ret;
}

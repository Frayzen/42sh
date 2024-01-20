#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <env/env.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "execs.h"
#include "tools/ast/ast.h"

extern char **environ;

int exec_for(struct ast_for *ast)
{
    assert(ast && AST(ast)->type == AST_FOR);
    if (!ast->nb_items)
        return 0;
    int ret = 0;
    for (int i = 0; i < ast->nb_items; i++)
    {
        // TODO variable assignment
        setenv(ast->name, ast->item_list[i], 1);
        ret = exec_list(AST_LIST(ast));
    }
    return ret;
}

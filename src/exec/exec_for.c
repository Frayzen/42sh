#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <env/env.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "execs.h"
#include "tools/ast/ast.h"

extern char **environ;

int exec_for(struct ast_for *ast)
{
    assert(ast && AST(ast)->type == AST_FOR);
    if (!ast->nb_items)
        return exec_list(AST_LIST(ast));
    int ret = 0;
    for (int i = 0; i < ast->nb_items; i++)
    {
        setenv(ast->name, ast->item_list[i], 1);
        printf("test type %d\n", AST(AST_LIST(ast))->type);
        ret = exec_list(AST_LIST(ast));
    }
    return ret;
}

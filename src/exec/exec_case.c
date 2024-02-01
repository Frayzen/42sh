#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "execs.h"
#include "parser/command/expander.h"
#include "parser/command/expansion.h"
#include "tools/ast/ast.h"

int exec_case(struct ast_case *ast)
{
    assert(ast && AST(ast)->type == AST_CASE);
    for (int i = 0; i < ast->nb_cond; i++)
    {
        for (int j = 0; ast->list_cond[i][j]; j++)
        {
            char *name = *expand(&ast->name);
            char *str = *expand(ast->list_cond[i][j]);
            if (!strcmp(name, str) || !strcmp("*", str))
                return exec_list(ast->cmds[i]);
        }
    }
    return 0;
}

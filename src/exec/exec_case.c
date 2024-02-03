#include <assert.h>
#include <fnmatch.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "execs.h"
#include "parser/command/expander.h"
#include "parser/command/expansion.h"
#include "tools/ast/ast.h"

int exec_case(struct ast_case *ast)
{
    assert(ast && AST(ast)->type == AST_CASE);
    char *name = expand_str(&ast->name);
    int ret = 0;
    for (int i = 0; i < ast->nb_cond; i++)
    {
        for (int j = 0; ast->list_cond[i][j]; j++)
        {
            char *pattern = expand_str(ast->list_cond[i][j]);
            if (!fnmatch(pattern, name, FNM_PATHNAME | FNM_NOESCAPE))
            {
                if (ast->cmds[i])
                {
                    free(pattern);
                    free(name);
                    return exec_list(ast->cmds[i]);
                }
            }
            free(pattern);
        }
    }
    free(name);
    return ret;
}

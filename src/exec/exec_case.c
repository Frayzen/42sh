#include <assert.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "execs.h"
#include "parser/command/expander.h"
#include "parser/command/expansion.h"
#include "tools/ast/ast.h"

bool match_pattern(char *pattern, char *str)
{
    int i = 0;
    while (pattern[i] == '*')
        i++;
    if (pattern[0] == '\0')
        return true;
    pattern += i;
    regex_t reg;
    regcomp(&reg, pattern, REG_EXTENDED);
    if (!regexec(&reg, str, 0, NULL, 0))
    {
        pattern -= i;
        regfree(&reg);
        return true;
    }
    pattern -= i;
    regfree(&reg);
    return false;
}

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
            if (match_pattern(pattern, name))
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

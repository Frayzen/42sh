#include <stddef.h>
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

enum status gr_shell_cmd(struct ast **ast)
{
    if (gr_if(ast) == OK)
        return OK;
    if (gr_while(ast) == OK)
    {
        printf("shoudlnt be =========================================\n");
        return OK;
    }
    return ERROR;
}

#include <stdio.h>
#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_command(struct ast **ast)
{
    if (gr_simple_command(ast) == OK)
        return OK;
    printf("gr_command mid\n\n");
    return gr_shell_cmd(ast);
}

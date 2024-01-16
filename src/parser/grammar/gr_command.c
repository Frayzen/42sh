#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_command(struct ast **ast)
{
    if (gr_simple_command(ast) == OK)
        return OK;
    printf("not simple\n");
    if (gr_shell_cmd(ast) == ERROR)
        return ERROR;
    printf("we got a shell cmd\n");
    enum status state = OK;
    while (state == OK)
    {
        printf("cmd loop\n");
        if (tok_peek()->terminal)
            break;
        state = gr_redir(ast);
    }
    return OK;
}

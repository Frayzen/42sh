
#include <stdio.h>

#include "env/env.h"
#include "exit/error_handler.h"
#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

enum status gr_input(struct ast **ast)
{
    if (tok_peek()->terminal)
    {
        tok_pop_clean();
        return OK;
    }
    CHECK_GOTO(gr_list(ast) == ERROR, error);
    tok_pop_clean();
    CHECK_GOTO(!tok_peek()->terminal, error);
    return OK;
error:
    comsume_all();
    destroy_ast(*ast);
    return ERROR;
}


#include <stdio.h>

#include "env/env.h"
#include "exit/error_handler.h"
#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

enum status gr_input(struct ast **ast)
{
    gr_list(ast);
    struct token *trm = tok_peek();
    tok_pop_clean();
    if (!trm->terminal)
        goto error;
    return OK;
error:
    comsume_all();
    destroy_ast(*ast);
    return ERROR;
}

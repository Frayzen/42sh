
#include <stdio.h>

#include "env/env.h"
#include "exit/error_handler.h"
#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/token/token.h"

enum status gr_input(struct ast **ast)
{
    GR_DBG_START(Input);
    gr_list(ast);
    struct token *trm = tok_peek();
    tok_pop_clean();
    if (!trm->terminal)
        goto error;
    GR_DBG_RET(OK);
error:
    comsume_all();
    destroy_ast(*ast);
    GR_DBG_RET(ERROR);
}

#include <stdio.h>
#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/token/token.h"

void consume_all(void)
{
    while (!tok_peek()->terminal)
        tok_pop_clean();
}

enum status gr_input(struct ast **ast)
{
    GR_DBG_START(Input);
    if (tok_peek()->terminal)
    {
        tok_pop_clean();
        GR_DBG_RET(OK);
    }
    CHECK_GOTO(gr_list(ast) == ERROR, error);
    struct token *trm = tok_peek();
    if (!trm->terminal)
        goto error;
    tok_pop_clean();
    GR_DBG_RET(OK);
error:
    consume_all();
    destroy_ast(*ast);
    GR_DBG_RET(ERROR);
}

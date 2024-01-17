
#include <stdio.h>

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

enum status gr_input(struct ast **ast)
{
    enum status state = gr_list(ast);
    printf("afterlist input\n");
    CHECK_GOTO(state == ERROR, error);
    struct token *trm = tok_peek();
    tok_pop();
    if (!trm->terminal)
    {
        // didn't put the CHECK_GOTO macro here ,because need to destroy_token
        // before goto error
        destroy_token(trm);
        goto error;
    }
    destroy_token(trm);
    return OK;
error:
    destroy_ast(*ast);
    return ERROR;
}

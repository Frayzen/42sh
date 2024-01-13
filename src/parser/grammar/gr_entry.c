
#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

enum status gr_input(struct ast **ast)
{
    enum status state = gr_list(ast);
    if (state == ERROR)
        goto error;
    struct token *trm = tok_peek();
    tok_pop();
    if (!trm->terminal)
    {
        destroy_token(trm);
        goto error;
    }
    destroy_token(trm);
    return OK;
error:
    destroy_ast(*ast);
    return ERROR;
}

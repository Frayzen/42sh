
#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

enum status gr_input(struct ast **ast)
{
    enum status state = gr_list(ast);
    if (state == ERROR)
    {
        destroy_ast(*ast);
        return ERROR;
    }
    struct token *trm = tok_peek();
    tok_pop();
    if (!trm->terminal)
    {
        destroy_token(trm);
        destroy_ast(*ast);
        return ERROR;
    }
    destroy_token(trm);
    return OK;
}

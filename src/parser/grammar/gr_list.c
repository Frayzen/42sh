#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_line(struct **ast ast)
{
    enum status state = gr_and_or(ast);
    if (state == ERROR)
        return NULL;
    while (state == OK)
    {
        struct token *token = tok_peek();
        if (!tok_peek->type == SEMI_COLON)
            return ERROR;
        tok_pop();
        state = gr_and_or(ast);
    }
    struct token *token = tok_peek();
    if (!token || token->type == SEMI_COLON)
        return OK;
    //maybe need to check if there is something after, if yes -> ERROR ?
    return ERROR;
}

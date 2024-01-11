#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"
#include "tools/ast/ast_utils.h"

struct ast *gr_end(void)
{
    struct token *tok = tok_peek();
    switch (tok->type) {
        case BSZERO:
        case NEWLINE:
            tok_pop();
        default:
            return NULL;
    }

    return init_ast(tok_peek());
}

struct ast *gr_input(void)
{
    struct ast *list = gr_list();
    struct ast *end = gr_end();
    if (!end)
    {
        destroy_ast(list);
        return NULL;
    }
    return add_child(list, end);
}

#include "exit/error_handler.h"
#include "lexer/token_saver.h"
#include "parser/tools/gr_tools.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/token/token.h"

void consume_all(void)
{
    while (tok_peek()->type != NEWLINE && tok_peek()->type != BSZERO)
        tok_pop_clean();
    tok_pop_clean();
}
/*
input =
list '\n'
| list EOF
| '\n'
| EOF
;
*/
enum status gr_input(struct ast **ast)
{
    GR_START(Input);
    enum status st = gr_list(ast);
    int empty = true;
    switch (st)
    {
    case NO_MATCH:
        break;
    case OK:
        empty = false;
        break;
    case ERROR:
        goto error;
    }
    struct token *end = tok_peek();
    if (end->type != BSZERO && end->type != NEWLINE)
        goto error;
    tok_pop_clean();
    if (empty)
    {
        if (tok_peek()->type == BSZERO)
            tok_pop_clean();
        GR_RET(NO_MATCH);
    }
    GR_RET(OK);
error:
    exit_gracefully(GRAMMAR_ERROR_ENTRY);
    consume_all();
    destroy_ast(*ast);
    GR_RET(ERROR);
}

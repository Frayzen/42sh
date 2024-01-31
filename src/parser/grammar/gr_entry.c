#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"
#include "tools/gr_tools.h"
#include "tools/token/token.h"

void consume_all(void)
{
    while (!tok_peek()->terminal)
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
    if (gr_list(ast) == ERROR)
        goto error;
    struct token *end = tok_peek();
    if (end->type != BSZERO && end->type != NEWLINE)
        goto error;
    tok_pop_clean();
    GR_RET(OK);
error:
    consume_all();
    destroy_ast(*ast);
    GR_RET(ERROR);
}

#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

enum status gr_simple_command(struct **ast ast)
{
    enum status state = gr_element(ast);
    if (state == ERROR)
        return NULL;
    while (state == OK)
    {
        state = gr_element(ast);
    }
    return OK;

}

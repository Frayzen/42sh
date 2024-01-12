#include <stdlib.h>
#include "lexer/token_saver.h"
#include "rules.h"
#include "tools/ast/ast.h"

struct ast *gr_entry(void)
{
    struct ast *ast = NULL;
    enum status state = gr_simple_command(&ast);
    if (state == ERROR || !tok_peek()->terminal)
    {
        tok_pop();
        destroy_ast(ast);
        return NULL;
    }
    return ast;
}

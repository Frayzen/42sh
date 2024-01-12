#include <stdlib.h>
#include "rules.h"
#include "tools/ast/ast.h"

struct ast *gr_entry(void)
{
    struct ast *ast = NULL;
    enum status state = gr_element(&ast);
    if (state == OK)
        return ast;
    return NULL;
}

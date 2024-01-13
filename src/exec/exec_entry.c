#include <stddef.h>

#include "execs.h"
#include "tools/ast/ast.h"

int exec_entry(struct ast *ast)
{
    if (ast == NULL)
        return 0;
    switch (ast->type)
    {
    case AST_LIST:
        return exec_list(ast);
    default:
        // TODO handle error
        return 1;
    }
}

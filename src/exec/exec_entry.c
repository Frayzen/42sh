#include <stddef.h>

#include "execs.h"
#include "exit/exit.h"
#include "tools/ast/ast.h"

void exec_entry(struct ast *ast)
{
    if (ast == NULL)
        return;
    switch (ast->type)
    {
    case AST_LIST:
        exec_list(ast);
        break;
    default:
        print_error(GRAMMAR_ERROR_ENTRY);
        break;
    }
}

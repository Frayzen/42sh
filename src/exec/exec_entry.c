#include <stddef.h>
#include <stdio.h>

#include "execs.h"
#include "exit/exit.h"
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
        print_error(GRAMMAR_ERROR_ENTRY);
        return 1;
    }
}

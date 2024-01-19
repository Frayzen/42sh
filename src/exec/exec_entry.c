#include <stddef.h>
#include <stdio.h>

#include "execs.h"
#include "exit/error_handler.h"
#include "tools/ast/ast.h"

int exec_entry(struct ast *ast)
{
    if (ast == NULL)
        return 0;
    switch (ast->type)
    {
    case AST_LIST:
        return exec_list(AST_LIST(ast));
        break;
    default:
        exit_gracefully(GRAMMAR_ERROR_ENTRY);
    }
    return -1; // shouldn't happpend
}

#include "execs.h"
#include "tools/ast/ast.h"

void exec_entry(struct ast *ast)
{
    switch (ast->type) {
        case AST_LIST:
            exec_list(ast);
            break;
        default:
            //TODO handle error
            break;

    }
}

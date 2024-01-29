#include "execs.h"
#include "tools/ast/ast.h"
#include "tools/funct_manager/funct_dict.h"

int exec_store_funct(struct ast_funct *ast)
{
    funct_dict_push(ast->name, &ast->body);
    return 0;
}

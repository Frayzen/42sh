#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>

#include "execs.h"
#include "tools/ast/ast.h"
#include "tools/funct_dict/funct_dict.h"

int exec_store_funct(struct ast_funct *ast)
{
    //    struct ast_sh *ast_dup = calloc(1, sizeof(struct ast_sh));
    //   ast_dup = memcpy(ast_dup, ast->body, sizeof(struct ast_sh));
    funct_dict_push(strdup(ast->name), ast->body);
    return 0;
}

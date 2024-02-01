#include "env/vars/var_dict.h"
#include "error_handler.h"
#include "tools/ast/ast.h"
#include "tools/fd_manager/fd_dictionnary.h"
#include "tools/funct_dict/funct_dict.h"
#include "tools/redirection/redirection.h"

void clean(struct ast *ast)
{
    destroy_ast(ast);
    dict_free();
    funct_dict_free();
    clean_variables();
    clean_debug_fds();
}

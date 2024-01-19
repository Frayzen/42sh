#include "error_handler.h"
#include "io_backend/backend_saver.h"
#include "lexer/token_saver.h"
#include "tools/ast/ast.h"
#include "tools/fd_manager/fd_dictionnary.h"
#include "tools/redirection/redirection.h"

void clean(struct ast *ast)
{
    destroy_ast(ast);
    dict_free();
    clean_debug_fds();
    clean_backend_saver();
    clean_token_saver();
}

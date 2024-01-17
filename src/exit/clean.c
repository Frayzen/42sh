#include "error_handler.h"
#include "io_backend/backend_saver.h"
#include "lexer/token_saver.h"
#include "tools/ast/ast.h"
#include "tools/fd_manager/fd_manager.h"

void clean(struct ast *ast)
{
    destroy_ast(ast);
    dict_free();
    clean_backend_saver();
    clean_token_saver();
}

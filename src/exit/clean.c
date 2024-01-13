#include "exit.h"
#include "io_backend/backend_saver.h"
#include "lexer/token_saver.h"
#include "tools/ast/ast.h"

void clean(struct ast *ast)
{
    destroy_ast(ast);
    clean_backend_saver();
    clean_token_saver();
}

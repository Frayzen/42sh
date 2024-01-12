#include <stdio.h>
#include <stdlib.h>

#include "io_backend/backend_saver.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast_utils.h"

int main(void)
{
    io_push("echo toto");
    struct ast *ast = NULL;
    gr_input(&ast);

    pretty_print_ast(ast);
    fflush(NULL);
    return 0;
}

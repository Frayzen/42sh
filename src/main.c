#include <stdio.h>
#include <stdlib.h>

#include "io_backend/backend_saver.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast_utils.h"
#include "exec/execs.h"
int main(void)
{
    io_push("ls ./");
    struct ast *ast = NULL;
    gr_input(&ast);
    exec_entry(ast);
    pretty_print_ast(ast);
    fflush(NULL);
    return 0;
}

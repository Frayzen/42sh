#include <stdio.h>
#include <stdlib.h>

#include "exec/execs.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"

int main(int argc, char *argv[])
{
    main_to_stream(argc, argv);
    struct ast *ast = NULL;
    gr_input(&ast);
    exec_entry(ast);
    destroy_ast(ast);
    return 0;
}

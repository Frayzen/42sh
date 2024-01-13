#include <stdio.h>
#include <stdlib.h>

#include "exec/execs.h"
#include "exit/exit.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"

int main(int argc, char *argv[])
{
    main_to_stream(argc, argv);
    struct ast *ast = NULL;
    gr_input(&ast);
    exec_entry(ast);
    /* pretty_print_ast(ast); */
    fflush(NULL);
    clean(ast);
    return 0;
}

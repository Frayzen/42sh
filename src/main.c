#include <stdio.h>
#include <stdlib.h>

#include "env/env.h"
#include "exec/execs.h"
#include "exit/exit.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

int main(int argc, char *argv[])
{
    main_to_stream(argc, argv);
    struct ast *ast = NULL;
        gr_input(&ast);
    if (get_env_flag()->print)
        pretty_print_ast(ast);
    exec_entry(ast);
    fflush(NULL);
    clean(ast);
    return 0;
}

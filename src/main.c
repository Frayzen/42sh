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
    set_ast_root(&ast);
    if (gr_input(&ast) == ERROR)
        exit_gracefully(GRAMMAR_ERROR_ENTRY);
    if (get_env_flag()->print)
        pretty_print_ast(ast);
    int ret = exec_entry(ast);
    fflush(NULL);
    clean(ast);
    return ret;
}

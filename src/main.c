#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "env/env.h"
#include "exec/execs.h"
#include "exit/exit.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/fd_manager/fd_manager.h"

int main(int argc, char *argv[])
{
    main_to_stream(argc, argv);
    struct ast *ast = NULL;
    int ret = 0;
    do
    {
        gr_input(&ast);
        if (get_env_flag()->print)
            pretty_print_ast(ast);
        ret = exec_entry(ast);
        assert(DICT->nb_entries == 0);
        fflush(NULL);
        destroy_ast(ast);
        ast = NULL;
    } while (!get_env_flag()->null_received && !get_env_flag()->str_input);
    clean(ast);
    return ret;
}

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "env/env.h"
#include "exec/execs.h"
#include "exit/error_handler.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/fd_manager/fd_dictionnary.h"

int main(int argc, char *argv[])
{
    main_to_stream(argc, argv);
    struct ast *ast = NULL;
    int ret = 0;
    do
    {
        if (gr_input(&ast) == ERROR)
        {
            ast = NULL;
            print_error(GRAMMAR_ERROR_ENTRY);
            continue;
        }
        if (get_env_flag()->pretty_print)
            pretty_print_ast(ast);
        ret = exec_entry(ast);
        assert(DICT->nb_entries == 0);
        fflush(NULL);
        destroy_ast(ast);
        ast = NULL;
    } while (!get_env_flag()->null_received);
    clean(ast);
    return ret;
}

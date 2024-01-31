#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "arg_parser/arg_parser.h"
#include "env/env.h"
#include "env/vars/vars.h"
#include "exec/execs.h"
#include "exit/error_handler.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"
#include "tools/fd_manager/fd_dictionnary.h"

int main(int argc, char *argv[])
{
    parse_args(argc, argv);
    srand(42); // Initialization, should only be called once.
    setup_debug_fds();
    struct ast *ast = NULL;
    swap_ast_root(&ast);
    init_env_vars();
    int ret = 0;
    do
    {
        STDOUT = STDOUT_FILENO;
        STDIN = STDIN_FILENO;
        if (gr_input(&ast) == ERROR)
        {
            ast = NULL;

            print_error(GRAMMAR_ERROR_ENTRY);
            ret = 2;
            continue;
        }
        if (get_env_flag()->pretty_print)
            debug_pretty_print(ast);
        if (ast)
            ret = exec_entry(ast);
        assert(DICT->nb_entries == 0);
        fflush(NULL);
        destroy_ast(ast);
        ast = NULL;
    } while (!get_env_flag()->null_received);
    clean(ast);
    return ret;
}

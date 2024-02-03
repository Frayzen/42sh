#include <stdio.h>

#include "exec/commands/execs_cmd.h"
#include "exec/execs.h"
#include "arg_saver/arg_saver.h"
#include "commands/execs_cmd.h"
#include "execs.h"
#include "exit/error_handler.h"
#include "tools/ast/ast.h"

int exec_sh(struct ast_sh *sh, char **argv)
{
    struct arg_info *old = NULL;
    if (argv)
    {
        int len = 0;
        while (argv[len])
            len++;
        old = new_arg_info(len, argv);
    }
    int ret = 0;
    struct ast *ast = sh->sh_cmd;
    switch (ast->type)
    {
    case AST_IF:
        ret = exec_condition(AST_IF(ast));
        break;
    case AST_WHILE:
    case AST_UNTIL:
        ret = exec_loop(AST_LOOP(ast));
        break;
    case AST_FOR:
        ret = exec_for(AST_FOR(ast));
        break;
    case AST_LIST:
        ret = exec_list(AST_LIST(ast));
        break;
    case AST_SUBSHELL:
        ret = exec_subshell(AST_SUBSHELL(ast));
        break;
    case AST_CASE:
        ret = exec_case(AST_CASE(ast));
        break;
    default:
        print_error(PIPE_NOT_FOUND);
        break;
    }
    if (argv)
        load_arg_info(old);
    return ret;
}

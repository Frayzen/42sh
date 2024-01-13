#include <stdio.h>
#include <stdlib.h>

#include "io_backend/backend_saver.h"
#include "lexer/token_saver.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

int main(int argc, char *argv[])
{
    main_to_stream(argc, argv);
    struct ast *ast = NULL;
    gr_input(&ast);
    //exec_entry(ast);
    pretty_print_ast(ast);
    fflush(NULL);
    destroy_ast(ast);
    clean_backend_saver();
    clean_token_saver();
    return 0;
}

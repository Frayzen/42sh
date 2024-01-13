#include <stdio.h>
#include <stdlib.h>

#include "exec/execs.h"
#include "exit/exit.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast.h"
#include "tools/ast/ast_utils.h"

/* int main(int argc, char *argv[]) */
int main(void)
{
    /* main_to_stream(argc, argv); */
    char *argv[] = {
        "fnieufn",
        "-c",
        "echo coucou uehouhf uehfi uhei uhfieu hifuhi i 'ehfieunf  biefue' e",
    };
    main_to_stream(3, argv);
    struct ast *ast = NULL;
    gr_input(&ast);
    exec_entry(ast);
    pretty_print_ast(ast);
    printf("%s\n", ast_to_str(ast));
    fflush(NULL);
    clean(ast);
    return 0;
}

#include <stdio.h>

#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"

int main(void)
{
    char *argv[4] = {
        "test",
        "-c",
        "echo toto;",
        NULL,
    };
    io_streamer_string(3, argv);
    struct ast *e = NULL;
    if(gr_input(&e) == ERROR)
    {
        printf("ERROR\n");
        return 1;
    }
    e++;
    /* pretty_print_ast(e); */
    return 0;
}

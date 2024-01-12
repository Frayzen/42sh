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
    struct ast *e = gr_entry();
    printf("%d\n", e->token->type);
}

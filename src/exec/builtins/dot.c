#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env/context.h"
#include "exec/execs.h"
#include "exit/error_handler.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"
#include "path_utils.h"

int builtin_dot(char **argv)
{
    char *path = argv[1];
    if (!strchr(path, '/'))
        path = look_file_path(path, "PATH", R_OK);
    if (access(path, R_OK))
    {
        print_error(INVALID_FILE_PATH);
        return 1;
    }
    struct context *old = new_context();
    io_streamer_file(path);
    int ret = 0;
    if (gr_input(AST_ROOT) == ERROR)
    {
        *AST_ROOT = NULL;
        print_error(GRAMMAR_ERROR_ENTRY);
        ret = 2;
    }
    else
        ret = exec_entry(*AST_ROOT);
    load_context(old);
    return ret;
}

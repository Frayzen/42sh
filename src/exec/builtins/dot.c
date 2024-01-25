#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env/env.h"
#include "execs.h"
#include "exit/error_handler.h"
#include "io_backend/backend_saver.h"
#include "io_backend/io_streamers.h"
#include "lexer/token_saver.h"
#include "parser/grammar/rules.h"
#include "tools/ast/ast_utils.h"
#include "tools/ring_buffer/ring_buffer.h"

#define PATH_MAX 4096

static char *create_path_from_filename(char *filename, char *path)
{
    char *new_path = malloc(sizeof(char) * PATH_MAX);
    strcpy(new_path, path);
    new_path = strcat(new_path, "/");
    new_path = strcat(new_path, filename);
    return new_path;
}

static char *look_file_path(char *filename)
{
    char *dup = strdup(getenv("PATH"));
    char *start = dup;
    char *end = NULL;
    do
    {
        end = strchr(start, ':');
        if (end)
        {
            end[0] = 0;
        }
        char *path_file = create_path_from_filename(filename, start);
        FILE *fd = fopen(path_file, "r");
        if (fd)
        {
            fclose(fd);
            free(dup);
            return path_file;
        }
        free(path_file);
        start = end + 1;
    } while (end);
    free(dup);
    return NULL;
}

int builtin_dot(char **argv)
{
    char *path = argv[1];
    if (!strchr(path, '/'))
    {
        path = look_file_path(path);
    }
    FILE *fd = fopen(path, "r");
    if (!fd)
        path = NULL;
    else
        fclose(fd);
    if (!path)
    {
        dprintf(STDERR, "error path file\n");
        return 1;
    }
    struct ringbuffer *new_backend_rb = rb_create(RB_CHAR, BACKEND_BUFFER_SIZE);
    struct ringbuffer *new_token_rb = rb_create(RB_TOKEN, 1);
    struct ringbuffer *old_backend_rb = swap_backend_buffer(new_backend_rb);
    struct ringbuffer *old_token_rb = swap_token_buffer(new_token_rb);
    FILE *old_fd = load_file(path);
    io_streamer_file(path);
    struct ast *new_ast = NULL;
    struct ast *old_ast = swap_ast_root(new_ast);
    int ret = 0;
    if (gr_input(&new_ast) == ERROR)
    {
        new_ast = NULL;
        print_error(GRAMMAR_ERROR_ENTRY);
        ret = 2;
    }
    ret = exec_entry(new_ast);
    destroy_ast(new_ast);
    swap_fd(old_fd);
    swap_ast_root(old_ast);
    swap_token_buffer(old_token_rb);
    swap_backend_buffer(old_backend_rb);
    rb_destroy(new_backend_rb);
    rb_destroy(new_token_rb);
    return ret;
}

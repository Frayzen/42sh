#define _POSIX_C_SOURCE 200809L
#include "io_backend/backend_saver.h"
#include "lexer/token_saver.h"
#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env/env.h"
#include "tools/ring_buffer/ring_buffer.h"

static char *create_path_from_filename(char *filename, char *path)
{
    char *new_path = malloc(sizeof(char) * PATH_MAX);
    strcpy(new_path, path);
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
        if (access(path_file, F_OK))
        {
            return path_file;
            free(dup);
        }
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
    else if (!access(path, F_OK))
        path = NULL;
    if (!path)
    {
        dprintf(STDERR, "error path file\n");
        return 1;
    }
    struct ringbuffer *new_backend_rb = rb_create(RB_CHAR, BACKEND_BUFFER_SIZE);
    struct ringbuffer *new_token_rb = rb_create(RB_TOKEN, 1);
    struct ringbuffer *old_backend_rb = swap_backend_buffer(new_backend_rb);
    struct ringbuffer *old_token_rb = swap_token_buffer(new_token_rb);
    // TODO build and execute the ast of the path
    swap_token_buffer(old_token_rb);
    swap_backend_buffer(old_backend_rb);
    rb_destroy(new_backend_rb);
    rb_destroy(new_token_rb);
    return 0;
}

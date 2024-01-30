#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "env/context.h"
#include "env/env.h"
#include "exec/execs.h"
#include "exit/error_handler.h"
#include "io_backend/io_streamers.h"
#include "parser/grammar/rules.h"

#define PATH_MAX 4096

/***
 * adds the filename to a path
 * @param filename the file to add
 * @param path the path to add the file to
 * @return the path with the filename
 */
static char *create_path_from_filename(char *filename, char *path)
{
    char *new_path = malloc(sizeof(char) * PATH_MAX);
    strcpy(new_path, path);
    new_path = strcat(new_path, "/");
    new_path = strcat(new_path, filename);
    return new_path;
}

/***
 * Looks through all the possible path to see if the file exists
 * @param ilename the file to look for
 * @return the valid path, NULL if doesn't exist
 */
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

// Return the FILE or NULL if the path is invalid
FILE *get_file(char *path)
{
    FILE *fd = fopen(path, "r");
    if (!fd)
        path = NULL;
    else
        fclose(fd);
    return fd;
}

int builtin_dot(char **argv)
{
    char *path = argv[1];
    if (!strchr(path, '/'))
        path = look_file_path(path);
    if (!get_file(path))
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

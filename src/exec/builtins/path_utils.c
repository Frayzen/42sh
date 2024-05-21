#define _POSIX_C_SOURCE 200809L
#include "path_utils.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "env/vars/vars.h"
#include "tools/definitions.h"
#include "unistd.h"

/***
 * adds the filename to a path
 * @param filename the file to add
 * @param path the path to add the file to
 * @return the path with the filename
 */
char *create_path_from_filename(char *filename, char *path)
{
    char *new_path = malloc(sizeof(char) * PATH_MAX);
    strcpy(new_path, path);
    new_path = strcat(new_path, "/");
    new_path = strcat(new_path, filename);
    return new_path;
}

char *look_file_path(char *filename, char *env_var, int file_type_mask)
{
    char *dup = strdup(read_var(env_var));
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
        if (!access(path_file, file_type_mask))
        {
            free(dup);
            return path_file;
        }
        free(path_file);
        start = end + 1;
    } while (end);
    free(dup);
    return NULL;
}

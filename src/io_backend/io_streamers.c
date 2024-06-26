#define _POSIX_C_SOURCE 200809L
#include "io_streamers.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "env/env.h"
#include "exit/error_handler.h"

static FILE *streamer = NULL;

FILE *swap_fd(FILE *new_file)
{
    if (new_file == NULL)
        return streamer;
    FILE *old = streamer;
    streamer = new_file;
    return old;
}

bool is_executable(char *path_to_file)
{
    if (access(path_to_file, X_OK) == true)
        return false;
    struct stat file_info;
    if (stat(path_to_file, &file_info) == true)
        return false;
    if (!S_ISREG(file_info.st_mode))
        return false;
    return true;
}

FILE *load_file(char *path_to_file)
{
    FILE *file = fopen(path_to_file, "r");
    if (!file)
    {
        exit_gracefully(INVALID_FILE_PATH);
    }
    if (!is_executable(path_to_file))
    {
        fclose(file);
        exit_gracefully(NO_EXEC_PERM);
    }
    fseek(file, 0, SEEK_SET);
    return swap_fd(file);
}

void io_streamer_file(char *path_to_file)
{
    if (access(path_to_file, F_OK))
        exit_gracefully(INVALID_FILE_PATH);
    if (access(path_to_file, R_OK))
        exit_gracefully(NO_EXEC_PERM);
    load_file(path_to_file);
}

void io_streamer_string(char *arg)
{
    char *buf = arg;
    FILE *file = fmemopen(buf, strlen(buf), "r");
    streamer = file;
    return;
}

void io_streamer_stdin(void)
{
    get_env_flag()->is_interactive = true;
    streamer = stdin;
}

char stream_next(void)
{
    char ret = fgetc(streamer);
    if (ret == -1)
        ret = '\0';
    fflush(streamer);
    return ret;
}

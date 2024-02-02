#define _POSIX_C_SOURCE 200809L
#include <linux/limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtins.h"
#include "builtins/path_utils.h"
#include "env/env.h"
#include "env/vars/vars.h"
#include "exit/error_handler.h"

static char *normalize(char *path)
{
    static char ret[PATH_MAX];
    memset(ret, 0, sizeof(char) * PATH_MAX);
    char *tmp = strtok(path, "/");
    char **path_elems = NULL;
    size_t fold_size = 0;
    while (tmp != NULL)
    {
        // NOT A POINT
        if (strcmp(tmp, "."))
        {
            if (!strcmp(tmp, ".."))
            {
                path_elems = realloc(path_elems, --fold_size * sizeof(char *));
            }
            else
            {
                // "CLASSIC" CASE
                path_elems = realloc(path_elems, ++fold_size * sizeof(char *));
                path_elems[fold_size - 1] = tmp;
            }
        }
        tmp = strtok(NULL, "/");
    }
    strcpy(ret, "/");
    for (size_t i = 0; i < fold_size - 1; i++)
    {
        strcat(ret, path_elems[i]);
        strcat(ret, "/");
    }
    strcat(ret, path_elems[fold_size - 1]);
    free(path_elems);
    return ret;
}

static char *compute_path(char *new)
{
    static char ret[PATH_MAX];
    char *pwd = read_var("PWD");
    int pwd_len = strlen(pwd);
    char *curpath = NULL;
    if (new[0] != '/' && new[0] != '.')
        curpath = look_file_path(new, "CDPATH", F_OK);
    if (!curpath)
        curpath = new;
    // Check for overflow
    int curpath_len = strlen(curpath);
    bool need_bslash = curpath[0] != '/' && pwd[pwd_len - 1] != '/';
    if (curpath_len + pwd_len + need_bslash > PATH_MAX)
        goto error;
    // STEP 7
    if (curpath[0] != '/')
    {
        strcpy(ret, pwd);
        if (ret[pwd_len - 1] != '/')
        {
            if (curpath_len + pwd_len + 1 > PATH_MAX)
                goto error;
            strcat(ret, "/");
        }
        strcat(ret, curpath);
    }
    else
        strcpy(ret, curpath);
    if (curpath != new)
        free(curpath);
    return ret;
error:
    free(curpath);
    return NULL;
}

int builtin_cd(char **argv)
{
    char *arg = NULL;
    // If no arguments, go to the HOME folder
    if (!argv[1])
        arg = read_var("HOME");
    else
    {
        // If the argument is -, go to the last folder (aka OLDPWD)
        if (!strcmp(argv[1], "-"))
        {
            arg = read_var("OLDPWD");
            dprintf(STDOUT, "%s\n", arg);
        }
        else
            // Otherwise, try to go to the argument
            arg = argv[1];
    }
    char *path = compute_path(arg);
    if (!path || chdir(path))
    {
        print_error(CD_BAD_PATH);
        return 1;
    }
    // Set the old pwd to the old pwd value and set the pwd value to the
    // current path
    assign_var("OLDPWD", read_var("PWD"));
    assign_var("PWD", normalize(path));
    return 0;
}

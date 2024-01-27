#define _POSIX_C_SOURCE 200809L
#include "arg_saver.h"
#include <stddef.h>
#include "tools/ast/ast.h"

char *build_argstr(char **argv, int argc) 
{
    int totalSize = 0;
    
    for (int i = 0; i < argc; i++)
        totalSize += strlen(argv[i]) + 1; 
    char *argstr = malloc(totalSize);
    if (argstr == NULL)
        exit(EXIT_FAILURE);
    int pos = 0;
    for (int i = 0; i < argc; i++)
    {
        char *arg = argv[i];
        int len = strlen(arg);
        memcpy(argstr + pos, arg, len);
        pos += len;
        if (i < argc - 1) 
            argstr[pos++] = ' ';
    }
    argstr[pos++] = '\0';

    return argstr;
}

struct arg_info *init_arg_info(char **argv, int argc)
{
    struct arg_info *arg_info = calloc(1, sizeof(struct arg_info)); 
    arg_info->argv = argv;
    arg_info->argstr = build_argstr(argv, argc);
    arg_info->argv = argv;
    return arg_info;
}

void destroy_arg_info(struct arg_info *arg_info)
{
    free(arg_info->argstr);
    free(arg_info);
}

static struct arg_info *old_arg_info = NULL;
static struct arg_info *cur_arg_info = NULL;


struct arg_info *new_arg_info(int argc, char **argv)
{
    assert(old_arg_info == NULL);
    old_arg_info = cur_arg_info;
    cur_arg_info = init_arg_info(argv, argc);
    return cur_arg_info;
}

struct arg_info *revert_arg_info(void)
{
    assert(old_arg_info != NULL);
    destroy_arg_info(cur_arg_info);
    cur_arg_info = old_arg_info;
    old_arg_info = NULL;
    return cur_arg_info; 
}

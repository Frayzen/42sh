#ifndef ARG_SAVER_H
#define ARG_SAVER_H
#include <stdlib.h>
#include <string.h>
struct arg_info
{
    char **argv;
    char *argstr;
    int argc;
};

char *build_argstr(char **argv, int argc);
struct arg_info *init_arg_info(char **argv, int argc);

struct arg_info *new_arg_info(int argc, char **argv);
struct arg_info *get_arg_info(void);

void load_arg_info(struct arg_info *old_arg_info);
#endif // !ARG_SAVER_H

#ifndef VARS_H
#define VARS_H
#define _POSIX_C_SOURCE 200809L
#include "env.h"
#include <stdio.h>
#define DBG_VAR(...)                                                          \
    if (get_env_flag()->debug_pipe)                                            \
        dprintf(DBG_OUT, __VA_ARGS__);

/*
 * set the variable named name to the value
 * @param name a string following the rules of the scl
 * @param value that is already expanded
 * return : the old value otherwise null
 * */
char *assign(char *name, char *value);

/*
 * retrieved a value from the env variables
 * @param name the name of the variable
 * returns the string value of the variable otherwise null
 */
char *retrieve(char *name);

#endif /* !VARS_H */

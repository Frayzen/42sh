#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "exec/execs.h"
#include "exit/error_handler.h"
#include "tools/str/string.h"

int get_nb_loop(int set)
{
    static int nb_loop = 0;
    if (set < 0)
        nb_loop = 0;
    else if (set)
        nb_loop = set;
    return nb_loop;
}

int get_continue(int set)
{
    static int cont = 0;
    if (set < 0)
        cont = 0;
    else if (set)
        cont = set;
    return cont;
}

int get_break(int set)
{
    static int brk = 0;
    if (set < 0)
        brk = 0;
    else if (set)
        brk = set;
    return brk;
}

int builtin_continue(char **argv)
{
    if (argv[1])
    {
        if (!is_number(argv[1]))
            exit_gracefully(CONT_BREAK_NOT_NUM);
        int cont = atoi(argv[1]);
        if (cont <= 0)
            exit_gracefully(CONT_BREAK_RANGE);
        else if (cont > NB_LOOPS)
            get_continue(NB_LOOPS);
        else
            get_continue(cont);
    }
    else
        get_continue(1);
    return 0;
}

int builtin_break(char **argv)
{
    if (argv[1])
    {
        if (!is_number(argv[1]))
            exit_gracefully(CONT_BREAK_NOT_NUM);
        int brk = atoi(argv[1]);
        if (brk <= 0)
            exit_gracefully(CONT_BREAK_RANGE);
        else if (brk > NB_LOOPS)
            get_break(NB_LOOPS);
        else
            get_break(brk);
    }
    get_break(1);
    return 0;
}

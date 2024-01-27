#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "exit/error_handler.h"
#include "exec/execs.h"
#include "tools/str/string.h"

int get_nb_loop(int set)
{
    static int nb_loop = 0;
    if (set >= 0)
        nb_loop = set;
    else if (set == -2)
        nb_loop = -1;
    return nb_loop;
}

int get_continue(int set)
{
    static int cont = 0;
    if (set >= 0)
        cont = set;
    else if (set == -2)
        cont = -1;
    return cont;
}

int get_break(int set)
{
    static int brk = 0;
    if (set)
        brk = set;
    else if (set == -2)
        brk = -1;
    return brk;
}

int builtin_continue(char **argv)
{
    if (argv[1])
    {
        int cont = atoi(argv[1]);
        if (!is_number(argv[1]) || NB_LOOPS == -1 || cont < 0)
            exit_gracefully(GRAMMAR_ERROR_ENTRY);
        else if (cont > NB_LOOPS)
            get_continue(NB_LOOPS);
        get_continue(cont);
    }
    get_continue(1);
    return 0;
}

int builtin_break(char **argv)
{
    if (argv[1])
    {
        int brk = atoi(argv[1]);
        if (!is_number(argv[1]) || NB_LOOPS == - 1 || brk < 0)
            exit_gracefully(GRAMMAR_ERROR_ENTRY);
        else if (brk > NB_LOOPS)
            get_break(NB_LOOPS);
        get_break(brk);
    }
    get_break(1);
    return 0;
}

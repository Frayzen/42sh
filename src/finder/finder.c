#define _XOPEN_SOURCE 500
#include "finder.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CHECK_SPECIAL_CHAR(Char)                                               \
    (((Char) == '\n' || (Char) == ';' || (Char) == '\'') ? 1 : 0)

static const char *const type_names[] = {
    [IF] = "if",       [THEN] = "then",      [ELIF] = "elif",
    [ELSE] = "else",   [FI] = "fi",          [SEMI_COLON] = ";",
    [BACK_N] = "\n",   [SINGLE_QUOTE] = "'", [ECHO] = "echo",
    [T_TRUE] = "true", [T_FALSE] = "false"
};

/***
 * check_reserved: checks if the word given is one of the reserved word
 * @param pending: a char*, the word we are inspecting
 * @return a boolean, 1 if it is a special word, 0 otherwise
 */
static int check_reserved(char *pending)
{
    char c = io_get_char();
    for (int i = 0; i < MAX_TYPE; i++)
    {
        if (!strcmp(pending, type_names[i]))
        {
            if (c != ' ')
                return 0;
            return 1;
        }
    }
    return 0;
}

void comments(void)
{
    char c = io_get_char();
    io_pop();
    while (c != '\n' && c != '\0')
    {
        c = io_get_char();
        io_pop();
    }
}

char *finder(void)
{
    char *pending =
        calloc(1, 1); // one character + terminating NULL to check with strcmp

    size_t size_pending = 0;
    char c = io_get_char();
    while (c == ' ')
    {
        io_pop();
        c = io_get_char();
    }
    pending[0] = c;
    size_pending++;
    pending[size_pending] = 0;
    io_pop();
    while (!check_reserved(pending))
    {
        c = io_get_char();
        if (c == '#')
            comments();
        else if (!(CHECK_SPECIAL_CHAR(c)))
        {
            pending = realloc(pending, size_pending + 1);
            pending[size_pending] = c;
            size_pending++;
            pending[size_pending] = 0;
            if (c == ' ' || c == '\0')
            {
                io_pop();
                return pending;
            }
        }
        else
            return pending;
        io_pop();
    }
    return pending;
}

/*
 right now, the single quotes give out singular tokens,
 but it can be modified to get one big token
 containing all that is between the quotes
 */

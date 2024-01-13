#include "io_backend/backend_saver.h"
#include "tools/token/token.h"
#define _XOPEN_SOURCE 500
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "finder.h"

#define IS_TERMINATING(Char) ((Char) == '\n' || (Char) == ';')

/***
 * checks if the word given is one of the reserved word
 */
static int check_reserved(char *pending)
{
    char c = io_peek();
    size_t i = 0;
    while (TOK_TYPE_LT[i])
    {
        if (!strcmp(pending, TOK_TYPE_LT[i]))
        {
            if (c != ' ')
                return 0;
            return 1;
        }
        i++;
    }
    return 0;
}

void comments(void)
{
    char c = io_peek();
    io_pop();
    while (!IS_TERMINATING(c) && c != '\0')
    {
        c = io_peek();
        io_pop();
    }
}

char *quotes(void)
{
    io_pop();
    char c = io_peek();
    char *pending = NULL;
    size_t size = 0;
    while (c && c != '\'')
    {
        io_pop();
        pending = realloc(pending, ++size);
        pending[size - 1] = c;
        c = io_peek();
    }
    pending = realloc(pending, size + 1);
    pending[size] = '\0';
    io_pop();
    return pending;
}

char *str_maker(void)
{
    char c = io_peek();
    size_t size_pending = 0;
    char *pending =
        calloc(2, 1); // one character + terminating NULL to check with strcmp
    pending[0] = c;
    pending[++size_pending] = 0;
    io_pop();
    if (c == '\n')
        return pending;
    while (!check_reserved(pending))
    {
        c = io_peek();
        if (c == ' ')
        {
            io_pop();
            return pending;
        }
        if (c == '#')
            comments();
        else if (!IS_TERMINATING(c))
        {
            size_pending++;
            pending = realloc(pending, size_pending + 1);
            pending[size_pending - 1] = c;
            pending[size_pending] = 0;
            if (c == ' ' || c == '\0')
            {
                io_pop();
                return pending;
            }
        }
        else
        {
            return pending;
        }
        io_pop();
    }
    return pending;
}

char *finder(void)
{
    char c = io_peek();
    if (c == ' ')
    {
        io_pop();
        return finder();
    }
    if (c == '#')
    {
        comments();
        return finder();
    }
    if (c == '\'')
    {
        return quotes();
    }
    return str_maker();
}

/*
 right now, the single quotes give out singular tokens,
 but it can be modified to get one big token
 containing all that is between the quotes
 */

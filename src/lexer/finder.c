#include <stdio.h>
#define _XOPEN_SOURCE 500
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "finder.h"
#include "io_backend/backend_saver.h"
#include "tools/token/token.h"

#define IS_TERMINATING(Char) ((Char) == '\n' || (Char) == ';')

char *append(char **pending, size_t *size, char c)
{
    *pending = realloc(*pending, ++(*size));
    (*pending)[*size - 1] = c;
    return *pending;
}

/***
 * checks if the word given is one of the reserved word
 */
static int check_reserved(char *pending, size_t size)
{
    if (!size)
        return 0;
    char c = io_peek();
    size_t i = 0;
    while (TOK_TYPE_LT[i])
    {
        if (strlen(TOK_TYPE_LT[i]) == size
            && !strncmp(pending, TOK_TYPE_LT[i], size))
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
    while (!IS_TERMINATING(c) && c != '\0')
    {
        c = io_peek();
        io_pop();
    }
}

char *quotes(void)
{
    char looking_for = io_peek();
    io_pop();
    char c = io_peek();
    char *pending = NULL;
    size_t size = 0;
    while (c && c != looking_for)
    {
        io_pop();
        append(&pending, &size, c);
        c = io_peek();
    }
    pending = realloc(pending, size + 1);
    append(&pending, &size, '\0');
    io_pop();
    return pending;
}


char *str_maker(void)
{
    size_t size_pending = 0;
    char *pending = NULL;
    char prev = '\0';
    char c = io_peek();
    if (c == ';' || c == '\n')
    {
        io_pop();
        append(&pending, &size_pending, c);
        return append(&pending, &size_pending, '\0');
    }
    c = '\0';
    while (!check_reserved(pending, size_pending))
    {
        prev = c;
        c = io_peek();
        if (c != ';' && c != '\n')
            io_pop();
        else
            return append(&pending, &size_pending, '\0');

        if (prev == '\\')
            goto append;
        if (c == ' ')
            return append(&pending, &size_pending, '\0');
        else if (c == '\\')
            continue;
        else if (c == '#' && prev == ' ')
            comments();
        else if (!IS_TERMINATING(c))
        {
        append:
            append(&pending, &size_pending, c);
            if (c == ' ' || c == '\0' || c == '\n')
            {
                return append(&pending, &size_pending, '\0');
            }
        }
        else
            return append(&pending, &size_pending, '\0');
    }
    return append(&pending, &size_pending, '\0');
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
    if (c == '\'' || c == '"')
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
